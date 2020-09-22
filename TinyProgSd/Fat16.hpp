#pragma once

#include <Arduino.h>

typedef struct {
    uint8_t first_byte;
    uint8_t start_chs[3];
    uint8_t partition_type;
    uint8_t end_chs[3];
    uint32_t start_sector;
    uint32_t length_sectors;
} __attribute((packed)) PartitionTable;

typedef struct {
    uint32_t fat_start;                 // FAT start position
    uint32_t data_start;                // data start position
    uint8_t sectors_per_cluster;        // cluster size in sectors
    uint16_t cluster;                   // current cluster being read
    uint32_t cluster_left;              // bytes left in current cluster
    uint32_t file_left;                 // bytes left in the file being read
} __attribute((packed)) Fat16State;

typedef struct {
    /*unsigned char jmp[3];
    char oem[8];*/
    uint16_t sector_size;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t number_of_fats;
    uint16_t root_dir_entries;
    uint16_t total_sectors_short; // if zero, later field is used
    uint8_t media_descriptor;
    uint16_t fat_size_sectors;
    /*unsigned short sectors_per_track;
    unsigned short number_of_heads;
    unsigned long hidden_sectors;
    unsigned long total_sectors_long;
    
    unsigned char drive_number;
    unsigned char current_head;
    unsigned char boot_signature;
    unsigned long volume_id;
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    unsigned short boot_sector_signature;*/
} __attribute((packed)) Fat16BootSectorFragment;

typedef struct {
    uint8_t filename[8];
    uint8_t ext[3];
    uint8_t attributes;
    uint8_t reserved[10];
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t starting_cluster;
    uint32_t file_size;
} __attribute((packed)) Fat16Entry;

namespace Fat16 {
    bool init();
    void seek(uint32_t offset);
    void read(uint16_t len);
}