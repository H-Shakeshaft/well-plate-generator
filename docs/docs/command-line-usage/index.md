# Using the CLI 

## Commands 
| Short Flag | Long Flag      | Description                                                                               | Example                                                                             |
|------------|----------------|-------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| `-h`       | `--help`       | Prints a list of commands and their usage                                                 |                                                                                     | 
| `-v`       | `--version`    | Prints version information about the application                                          |                                                                                     |
| `-t`       | `--template`   | Path to template file used to simulate data                                               | ```-t /Users/henryshakeshaft/Documents/screening-plate-templates/96-lr.tplx```      |
| `-n`       | `--num-plates` | Number of plates to generate                                                              | ```-n 100```                                                                        |
| `-e`       | `--equation`   | The equation used to simulate data <br/> valid values:`EC50`, `IC50` or `XC50`            | ```-e EC50```                                                                       |
| `-f`       | `--format`     | The output format that generated data should be saved as. Valid formats are `csv` & `tsv` | ```-f csv```                                                                        |
| `-o`       | `--output`     | Path to directory where generated data should be saved                                    | ```-o /Users/henryshakeshaft/Documents/screening-plate-templates/generated-data```  | 


!!! example "Generating 20 CSV plates with EC50 shape" 

    === "Unix"
        ```bash
        wellgen -t $HOME/well-gen/templates/96-lr.tplx -n 20 -e EC50 -f csv -o $HOME/well-gen/generated
        ```

    === "Windows"
        ```bash
        wellgen -t %USERPROFILE%/well-gen/templates/96-lr.tplx -n 20 -e EC50 -f csv -o %USERPROFILE%/well-gen/generated
        ```
