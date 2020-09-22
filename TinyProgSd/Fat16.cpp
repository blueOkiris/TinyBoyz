#include <Arduino.h>
#include "Sd.hpp"
#include "Fat16.hpp"

int16_t i;

static uint8_t buffer[32];
static Fat16State state;
#define FAT16_BOOT_OFFSET   11
#define FAT16_part          ((PartitionTable *) ((void *) buffer))
#define FAT16_boot          ((Fat16BootSectorFragment *) ((void *) buffer))

static uint32_t sector;
static uint16_t pos;

bool Fat16::init() {
    sector = pos = 0;
    if(!SdCard::init()) {
        return false;
    }
    
    seek(0x000001BE);

    for(i = 0; i < 4; i++) {
        read(sizeof(PartitionTable));
        if(FAT16_part->partition_type == 4
                || FAT16_part->partition_type == 6
                || FAT16_part->partition_type == 14) {
            break;
        }
    }

    if(i == 0) {
        return false;
    }

    state.fat_start = 512 * FAT16_part->start_sector;   // temporary

    seek(state.fat_start + FAT16_BOOT_OFFSET);
    read(sizeof(Fat16BootSectorFragment));

    if(FAT16_boot->sector_size != 512)
        return false;
    
    state.fat_start += FAT16_boot->reserved_sectors * 512;

    uint32_t root_start = state.fat_start
        + (uint32_t) FAT16_boot->fat_size_sectors
        * (uint32_t) FAT16_boot->number_of_fats * 512;
    state.data_start = root_start + sizeof(Fat16Entry)
        * (uint32_t) FAT16_boot->root_dir_entries;
    state.sectors_per_cluster = FAT16_boot->sectors_per_cluster;

    // Prepare for fopen(), cluster is not needed
    state.file_left = FAT16_boot->root_dir_entries * sizeof(Fat16Entry);
    state.cluster_left = 0xFFFFFFFF;                    // avoid lookup w/ root 

    seek(root_start);

    return true;
}

void Fat16::seek(uint32_t offset) {
    sector = offset >> 9;
    pos = offset & 511;
}

void Fat16::read(uint16_t len) {
    SdCard::read(sector, pos, buffer, len);
    pos += len;

    if(pos == 512) {
        pos = 0;
        sector++;
    }
}
