import time
import numpy as np
import matplotlib.pyplot as plt

def multiply_random_square_matrices(N: int) -> float:
    A = np.random.rand(N, N).astype(np.float32)
    B = np.random.rand(N, N).astype(np.float32)

    start_time = time.time()
    C = A @ B
    end_time = time.time()
    execution_time = end_time - start_time

    num_ops = 2 * N ** 3
    flops = num_ops / execution_time
    gflops = flops / 1e9
    print(f"GFLOP/s: {gflops:.2f}")

    return gflops

if __name__=="__main__":
    num_samples = 10
    sizes = [100, 1000, 5000, 10000]
    gflops = []
    for size in sizes:
        samples = []
        for i in range(num_samples):
            samples.append(multiply_random_square_matrices(3000))
        gflops.append(sum(samples) / num_samples)
    
    fig, ax = plt.subplots()
    ax.plot(sizes, gflops)
    ax.set_xlabel("Matrix Size N")
    ax.set_ylabel("GFLOP/s")
    fig.savefig("numpy_matmul_gflops.png")

    