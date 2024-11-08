# Time Series Analysis - Chemical Mechanical Polishing

This project analyzes time series data from a Chemical Mechanical Polishing (CMP) process, which is used in the semiconductor industry to polish wafer surfaces. The goal is to predict and explain the average material removal rate of wafers based on historical data.

## Project Overview

The application, developed in C, performs a series of data analysis tasks on time series data, enabling users to preprocess, analyze, and make basic predictions on CMP process data.

## Features

The project provides several key functionalities:
1. **Descriptive Statistics**: Calculates central (mean, median) and dispersion (minimum, maximum, standard deviation, quartiles) statistics for each time series. Outputs are saved to a text file.
2. **Outlier Detection**: Detects and removes outlier values using a kσ rule (where k = 2 or 3, and σ is the standard deviation).
3. **Empirical Distributions**: Constructs histograms by dividing each time series range into equal bins and calculating the frequency of observations within each bin.
4. **Data Aggregation by Wafer**: Aggregates time series data by averaging values for each wafer. The aggregated data is saved to a text file.
5. **Correlation and Prediction (Optional)**: Calculates correlations between aggregated features and the material removal rate and provides an optional linear regression model for predicting the rate.

## Compilation and Execution

To compile and execute the project, use the following commands:

```bash
gcc projet.c -o projet -lm
./projet
```

## Project Background

The data analyzed in this project comes from a 2016 competition organized by PHM Society, focused on predictive maintenance in semiconductor manufacturing. Specifically, it addresses CMP, a widely used process in the semiconductor industry.

This project was developed as part of the **Algorithms and Programming** course at ISMIN.

## Repository Structure

- `projet.c`: Main C source file containing the implementation of the project.
- `data/`: Folder (to be created) for storing input time series data files.
- `output/`: Folder (to be created) for storing output files (e.g., statistics, cleaned data).

## Future Enhancements

Some potential improvements for future work include:
- Adding advanced prediction models (e.g., polynomial regression, machine learning algorithms).
- Enabling dynamic binning for histograms based on data distribution.
- Including a command-line interface for more user-friendly interaction.

## License

This project is open-source and available under the [MIT License](LICENSE).
