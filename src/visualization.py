from nilearn import plotting
import matplotlib.pyplot as plt
import matplotlib.colors as clr
from sklearn.preprocessing import MinMaxScaler
import numpy as np


def load_adjcency_mat(edge_ls_f: str) -> np.ndarray:
    return np.loadtxt(edge_ls_f, dtype=float)


def load_vertex_rank(rank_f: str) -> np.ndarray:
    return np.loadtxt(rank_f, dtype=float)


def load_coords(coords_ls: str) -> dict:
    with open(coords_ls, 'r') as f:
        content = f.readlines()

    coords_dict = {}
    curr_roi = ""
    for ln in content:
        curr_ln = ln[:-1].split(',')
        if (len(curr_ln) == 2):  # the ROI line
            coords_dict[curr_ln[0]] = []
            curr_roi = curr_ln[0]
        else:
            coords_dict[curr_roi].append([float(c) for c in curr_ln])

    return coords_dict


def _coords2ls(coords: dict, adjust_factor) -> list:
    coords_ls = []
    for _, val in coords.items():
        for v in val:
            coords_ls.append([v[0], -v[1] + adjust_factor, v[2]])
    return coords_ls


def plot_vertex_brain_3D(rank_scores, coords: dict, save_f:str):
    coords_ls = _coords2ls(coords, 5)

    cmap = plt.cm.Reds
    norm = clr.Normalize()
    rank_color = cmap(norm(rank_scores))
    rank_size = MinMaxScaler((0, 40)).fit_transform(rank_scores.reshape(-1, 1)).flatten()

    # plot interactive 3D brain
    view = plotting.view_markers(coords_ls, marker_size=rank_size, marker_color=rank_color)
    # view.open_in_browser()
    view.save_as_html(save_f)


def plot_connectivity_brain_3D(adj_mat, coords: dict, save_f: str):
    # plot interactive 3D brain
    coords_ls = _coords2ls(coords, 5)
    palette = ["blue", "green", "black", "purple", "aqua"]
    node_color = []
    i = 0
    for _, val in coords.items():
        node_color += [palette[i] for _ in range(len(val))]
        i += 1

    view = plotting.view_connectome(adj_mat, coords_ls, node_color=node_color, edge_threshold=0, edge_cmap="bwr", linewidth=2.0)
    # view.open_in_browser()
    view.save_as_html(save_f)


def main():
    ##=================== load data
    graph_img = "../data/results/img_girvan_newman.txt"
    graph_pcp = "../data/results/pcp_girvan_newman.txt"

    rank_img = "../data/results/img_ranks.txt"
    rank_pcp = "../data/results/pcp_ranks.txt"

    coords_ls = "../data/coords.txt"

    # things needed:
    adj_mat_img = load_adjcency_mat(graph_img)  # the adjacency matrix for imagery after GN
    adj_mat_pcp = load_adjcency_mat(graph_pcp)  # the adjacency matrix for perception

    rank_arr_img = load_vertex_rank(rank_img)  # an array of rank scores corresponding to the vertices for imagery
    rank_arr_pcp = load_vertex_rank(rank_pcp)  # an array of rank scores corresponding to the vertices for perception

    coords = load_coords(coords_ls)  # the coordinates of all vertices, can be from the original coords.txt

    ##==================== plot stuff

    # rank visualization
    plot_vertex_brain_3D(rank_arr_pcp, coords, "../data/results/vis_rank_pcp.html")
    plot_vertex_brain_3D(rank_arr_img, coords, "../data/results/vis_rank_img.html")

    # connectivity visualization
    plot_connectivity_brain_3D(adj_mat_pcp, coords, "../data/results/vis_graph_pcp.html")
    plot_connectivity_brain_3D(adj_mat_img, coords, "../data/results/vis_graph_img.html")


if __name__ == "__main__":
    main()
