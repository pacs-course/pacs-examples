import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter

import re
import os


def plot_sir_curve():
    df = pd.read_csv("output.csv")
    plt.figure()
    plt.plot(df["time"], df["n_susceptible"], label="Susceptible")
    plt.plot(df["time"], df["n_infected"], label="Infected")
    plt.plot(df["time"], df["n_recovered"], label="Recovered")
    plt.ylabel("No. of agents")
    plt.xlabel("Time [days]")
    plt.legend()
    plt.savefig("output.png")


def plot_snapshots():
    ls = os.listdir(".")
    snapshots = [file for file in ls if re.match(r"snapshot\.\d+\.csv", file)]
    snapshots.sort()

    fig, ax = plt.subplots()

    def animate(i):
        print("Animating step", i, "of", len(snapshots))
        df = pd.read_csv(snapshots[i])
        ax.clear()
        ax.set_xlim(0, 1)
        ax.set_ylim(0, 1)
        sct = ax.scatter(df["x"], df["y"], c=plt.cm.jet(df["state"] / 2))
        return sct,

    interval = 200
    ani = FuncAnimation(
        fig, animate, interval=interval, blit=True, repeat=True, frames=len(snapshots)
    )
    ani.save("sir.gif", dpi=100, writer=PillowWriter(fps=1000/interval))


if __name__ == "__main__":
    plot_sir_curve()
    plot_snapshots()
