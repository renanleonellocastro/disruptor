#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np
import sys

def read_values_from_file(filename):
    with open(filename, 'r') as file:
        values = [int(line.strip()) for line in file]
    return values

def calculate_percentiles(values):
    p50 = np.percentile(values, 50)
    p90 = np.percentile(values, 90)
    p95 = np.percentile(values, 95)
    p99 = np.percentile(values, 99)
    return p50, p90, p95, p99

def plot_values(values, title, filename):
    plt.figure(figsize=(10, 6))
    plt.plot(values, marker='o', linestyle='-', color='b')
    plt.title(title)
    plt.xlabel("Index")
    plt.ylabel("Microsecond Value")
    plt.ylim(0, 50000)  # Fixed Y-axis scale from 0 to 50000
    plt.grid(True)
    
    # Save plot to file
    plt.savefig(filename)

def main(filename):
    # Read and sort values
    values = read_values_from_file(filename)

    # Plot and save values to a file
    plot_values(values, "Latency in Microseconds", "plot.png")

    # Sort values
    values.sort()

    # Plot and save values to a file
    plot_values(values, "Sorted Latency in Microseconds", "sorted_plot.png")

    # Calculate percentiles
    p50, p90, p95, p99 = calculate_percentiles(values)
    min_val = min(values)
    max_val = max(values)
    total_latency = sum(values)/1000000.0

    # Print calculated statistics
    print(f"Min value: {min_val}")
    print(f"Max value: {max_val}")
    print(f"p50 (50th percentile): {p50}")
    print(f"p90 (90th percentile): {p90}")
    print(f"p95 (95th percentile): {p95}")
    print(f"p99 (99th percentile): {p99}")

# Execute main
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python plot_microseconds.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    main(filename)
