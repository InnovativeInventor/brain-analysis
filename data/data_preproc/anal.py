import numpy as np
import pandas as pd
import os

img_run_ls = [[5, 5, 5, 5],
              [8, 6, 6],
              [3, 9, 8]]

pcp_run_ls = [[8, 8, 8],
              [8, 6, 8],
              [8, 8, 8]]

roi_ls = ["HIPPO", "PPA", "IPS", "RSC", "OPA"]
TR = 2


def extract_coords():
    dat_dir = "../sub01/img_ses01/timeseries_data"
    with open("coords.txt", "a+") as f:
        for roi in roi_ls:
            coords = np.loadtxt(f"{dat_dir}/sub01_r01_{roi}.txt", dtype=str)[:, :3]
            f.write(f"{roi}, {coords.shape[0]}\n")
            for vox_i in range(coords.shape[0]):
                f.write(','.join(coords[vox_i, :]) + '\n')


def load_behav_img(sub, session_num, run):
    "sub-01_ses-imagery01_task-imagery_run-01_events"
    dat_dir = f"../sub0{sub}/img_ses0{session_num}/behav"
    fname = f"sub-0{sub}_ses-imagery0{session_num}_task-imagery_run-0{run}_events.tsv"

    df = pd.read_csv(os.path.join(dat_dir, fname), sep='\t', header=0)
    df = (df.query("evaluation > 2.0")).query("event_type == 2.0")
    onset = [[t + i for i in range(4)] for t in (df["onset"]/TR)]
    return np.asarray(onset, dtype=int).flatten()


def load_behav_pcp(sub, session_num, run):
    "sub-01_ses-perceptionNaturalImageTest01_task-perception_run-01_events"
    dat_dir = f"../sub0{sub}/pcp_ses0{session_num}/behav"
    fname = f"sub-0{sub}_ses-perceptionNaturalImageTest0{session_num}_task-perception_run-0{run}_events.tsv"

    df = pd.read_csv(os.path.join(dat_dir, fname), sep='\t', header=0)
    df = df.query("event_type == 1.0")
    onset = [[t + i for i in range(4)] for t in (df["onset"] / TR)]
    return np.asarray(onset, dtype=int).flatten()


def load_dat_img(sub, session_num, run, roi):
    dat_dir = f"../sub0{sub}/img_ses0{session_num}/timeseries_data"
    fname = f"sub0{sub}_r0{run}_{roi}.txt"
    dat = np.loadtxt(os.path.join(dat_dir, fname), dtype = float)[:, 3:]

    return dat


def load_dat_pcp(sub, session_num, run, roi):
    dat_dir = f"../sub0{sub}/pcp_ses0{session_num}/timeseries_data"
    fname = f"sub0{sub}_r0{run}_{roi}.txt"
    dat = np.loadtxt(os.path.join(dat_dir, fname), dtype = float)[:, 3:]

    return dat


def extract_vox(sub, ses_name):

    all_vox = []
    runs = img_run_ls[sub - 1] if ses_name == "img" else pcp_run_ls[sub - 1]
    for roi in roi_ls:
        roi_vox = []
        for ses in range(len(runs)):
            for run in range(runs[ses]):
                print(roi, f"sub: {sub}, ses: {ses}, run: {run}")
                if ses_name == "img":
                    onset = load_behav_img(sub, ses+1, run+1)
                    dat = load_dat_img(sub, ses+1, run+1, roi)
                else:
                    onset = load_behav_pcp(sub, ses + 1, run + 1)
                    dat = load_dat_pcp(sub, ses + 1, run + 1, roi)

                roi_vox.append(dat[:, onset])
        all_vox.append(np.hstack(roi_vox))
    return np.vstack(all_vox)


def main():
    # img first
    # full_mat = []
    # for sub in range(3):
    #     all_vox = extract_vox(sub+1, "img")
    #     full_mat.append(np.corrcoef(all_vox))
    # mean_mat = np.abs(np.mean(np.asarray(full_mat), axis=0))
    # np.save("full_img.npy", mean_mat)
    img = np.load("full_img.npy")
    img = np.tril(img, -1)
    cutoff = np.percentile(img[img != 0], 97)
    print(cutoff)
    # with open("pcp_edge_list.txt", 'w') as f:
    #     f.write("vox1,vox2,weight\n")
    #     for i in range(img.shape[0]):
    #         for j in range(i):
    #             if img[i, j] >= cutoff:
    #                 f.write(f"{i},{j},{img[i, j]}\n")


    # print(img.shape[0] * img.shape[0])
    # print(np.sum(img < 0.34))


    # pcp next
    # full_mat = []
    # for sub in range(3):
    #     all_vox = extract_vox(sub + 1, "pcp")
    #     full_mat.append(np.corrcoef(all_vox))
    # mean_mat = np.mean(np.asarray(full_mat), axis=0)
    # np.save("full_pcp.npy", mean_mat)
    # pcp = np.load("full_pcp.npy")


if __name__ == "__main__":
    # load_behav()
    # extract_coords()
    main()
    # load_behav_img(1, 1, 1)
    # load_behav_pcp(1, 1, 1)
    # load_dat_pcp(1,1,1, "PPA")
    # extract_vox(1, "img")