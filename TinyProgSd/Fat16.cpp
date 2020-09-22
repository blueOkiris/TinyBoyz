#include <Arduino.h>
#include "Sd.hpp"
#include "Fat16.hpp"

int16_t i;

static Fat16State state;
uint8_t fileBuffer[32];
#define FAT16_BOOT_OFFSET   11
#define FAT16_part          ((PartitionTable *) ((void *) fileBuffer))
#define FAT16_boot          ((Fat16BootSectorFragment *) ((void *) fileBuffer))
#define FAT16_entry         ((Fat16Entry *) ((void *) fileBuffer))
#define FAT16_ushort        ((uint16_t *) ((void *) fileBuffer))

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
    SdCard::read(sector, pos, fileBuffer, len);
    pos += len;

    if(pos == 512) {
        pos = 0;
        sector++;
    }
}

bool Fat16::openFile(char *fileName, char *ext) {
    do {
        readFile(sizeof(Fat16Entry));
        for(i = 0; i < 8; i++) {
            if(FAT16_entry->filename[i] != fileName[i]) {
                break;
            }
        }
        if(i < 8) {
            continue;                                   // File lookup failed
        }

        for(i = 0; i < 3; i++) {
            if(FAT16_entry->ext[i] != ext[i]) {
                break;
            }
        }
        if(i < 3) {
            continue;
        }

        state.cluster = FAT16_entry->starting_cluster;
        state.cluster_left = (uint32_t) state.sectors_per_cluster * 512;

        if(FAT16_entry->filename[0] == 0x2E
                || FAT16_entry->attributes & 0x10) {
            state.file_left = 0xFFFFFFF;
        } else {
            state.file_left = FAT16_entry->file_size;
        }

        seek(
            state.data_start + (uint32_t) (state.cluster - 2)
            * (uint32_t) state.sectors_per_cluster * 512
        );
        return true;
    } while(state.file_left);

    return false;
}

void Fat16::readFile(uint8_t len) {
    if(state.file_left == 0) {
        return 0;
    }

    if(state.cluster_left == 0) {
        seek(state.fat_start + (uint32_t) state.cluster * 2);
        read(2);

        state.cluster = FAT16_ushort[0];
        state.cluster_left = (uint32_t) state.sectors_per_cluster * 512;

        if(state.cluster == 0xFFFF) {
            state.file_left = 0;
            return;
        }

        seek(
            state.data_start + (uint32_t) (state.cluster - 2)
            * (uint32_t) state.sectors_per_cluster * 512
        );
    }

    if(len > state.file_left) {
        len = state.file_left;
    }
    if(len > state.cluster_left) {
        len = state.cluster_left;
    }
    read(len);
    state.file_left -= len;
    state.cluster_left -= len;
}

bool Fat16::eof() {
    return state.file_left == 0;
}
