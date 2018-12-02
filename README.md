# MetadataManagementInDFS

### Compilation

- To compile the project, go into the directory ``src`` and run the command ``make``.
- To clean up the executable and intermediate files, run ``make clean``.

### Launching

#### Server

##### Master Server

run ``./server PORT`` 

##### Slave Server

run ``./server PORT MASTER_ADDR MASTER_PORT``

#### Client

run ``./client MASTER_ADDR MASTER_PORT``

### Usage

Available commands:

- ``pwd``
- ``ls``
- ``rm``
- ``touch``
- ``cd``
- ``mv``
- ``mkdir``
- ``stat``
- ``checkservers``

