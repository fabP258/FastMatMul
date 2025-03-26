import subprocess
from pathlib import Path
import matplotlib.pyplot as plt

def build():
    print("Executing build command: make build")
    result = subprocess.run(["make", "clean"], capture_output=True, text=True)
    print(result.stdout)

def run():
    print("Executing program")
    result = subprocess.run(["make", "run"], capture_output=True, text=True)
    print(result.stdout)

def plot():
    if not Path("results.csv").is_file():
        print("ERROR: results.csv file not available.")
        return
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

    fig, ax = plt.subplots()

    for algo_idx in range(len(csv_data["columns"])-1):
        ax.plot(csv_data["data"][0], csv_data["data"][algo_idx+1], label=csv_data["columns"][algo_idx+1], marker='o')
    ax.legend()
    ax.set_xlabel("matrix size (M=N=K)")
    ax.set_ylabel("GxOP/s")
    fig.savefig("benchmark.png")

def main():
    build()
    run()
    # TODO: run Numpy benchmark here
    plot()

if __name__ == "__main__":
    main()