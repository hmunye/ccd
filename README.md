<div align="center">

  # ccd

</div>

`ccd` is a simple implementation of the `xxd` hex dumper utility that reads a 
file and prints its contents in a structured hexadecimal format to stdout.

## Prerequisites

Ensure you have the following installed:

- `gcc`
- `make`

## Build

Run the following command to see available build options:

```bash
make help
```

## Usage

```bash
./ccd [options] [filename]
```

## Options

- `-h`, `--help`:
Show the help message and a summary of program options.

- `-c cols`:
Specify the number of octets to display per line. The default is 16.
