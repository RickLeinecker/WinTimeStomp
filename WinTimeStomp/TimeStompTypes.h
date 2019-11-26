#pragma once

typedef LONG NTSTATUS;

typedef enum _FILE_INFORMATION_CLASS
{
    FileBasicInformation = 4, 
    FileStandardInformation = 5, 
    FilePositionInformation = 14, 
    FileEndOfFileInformation = 20, 
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct _FILE_BASIC_INFORMATION {                    
    LARGE_INTEGER CreationTime;							// Created             
    LARGE_INTEGER LastAccessTime;                       // Accessed    
    LARGE_INTEGER LastWriteTime;                        // Modifed
    LARGE_INTEGER ChangeTime;                           // Entry Modified
    ULONG FileAttributes;                                   
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
