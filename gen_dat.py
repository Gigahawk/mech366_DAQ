import os
import pandas as pd
import numpy as np

csv_cmd = "libreoffice --headless --convert-to csv --outdir data_csv data_src/*.ods"
os.system(csv_cmd)

csv_files = [f for f in os.listdir("data_csv") if f.endswith(".csv")]

for f in csv_files:
    out_file = os.path.splitext(f)[0] + ".dat"
    df = pd.read_csv(os.path.join("data_csv", f))
    time =  np.array(df["time (ms)"])
    speed = np.array(df["speed"])
    t_check = np.arange(len(time))
    if not all(time == t_check):
        print(f"Skipping {f}: a value must be specified for every time step")
        continue
    speed = speed.astype(np.uint8)
    print(f"Saving {out_file}")
    with open(os.path.join("data", out_file), "wb") as fh:
        fh.write(speed.tobytes())
