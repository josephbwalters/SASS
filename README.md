# SASS
Repository for all code related to UCF CECS Senior Design Project (S.A.S.S. -- Fall 2018 - Spring 2019)

## Installation:
- Clone Repo
- Open SASS, MSS, and DSS seperately in Code Composer Studio (CCS)

## Building Software
### SASS
- Build using CCS

### MMWAVE
- Build DSS in CCS
- Then build MSS in CCS

## Flashing Firmware onto devices
*Note: This is done after building each component*
### SASS
- Using CCS and a MSP432P401R EVM (Dev. Kit) you can flash the system externally

### MMWAVE
- Take the mmwave .bin file from the MSS' Debug folder and use UniFlash to flash the mmwave EVM
