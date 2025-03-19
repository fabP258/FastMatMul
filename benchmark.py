"""
TODO:
- Execute make command to build executable
- Load csv data
- Run Numpy benchmark program in Python
- Plot results (matplotlib)
"""

import subprocess
import matplotlib.pyplot as plt

def build():
    return None

def run():
    print("Calling Executable to run benchmark ...")
    result = subprocess.run(["matmul.exe"], capture_output=True, text=True)
    print(result.stdout)

def plot():
    # TODO: check if results exists
    csv_data = {}
    with open("results.csv", "r") as f:
        for i, line in enumerate(f):
            line = line.strip()
            data = line.split(',')
            if i == 0:
                csv_data.update({"columns": data})
                csv_data.update({"data": [[] for _ in range(len(data))]})
                continue
            for col_idx, val in enumerate(data):
                dividend = 1e9 # convert to GFLOP/s
                if col_idx == 0:
                    dividend = 1
                csv_data["data"][col_idx].append(float(val) / dividend)
    
    print(csv_data["columns"])
    print(csv_data["data"])

    fig, ax = plt.subplots()

    for algo_idx in range(len(csv_data["columns"])-1):
        ax.plot(csv_data["data"][0], csv_data["data"][algo_idx+1], label=csv_data["columns"][algo_idx+1], marker='o')
    
    ax.legend()
    fig.savefig("benchmark.png")

def main():
    build()
    run()
    # TODO: run Numpy benchmark here
    plot()
    
    return None

if __name__ == "__main__":
    main()