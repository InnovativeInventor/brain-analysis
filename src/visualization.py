from nilearn import plotting
import numpy as np


def load_adjcency_mat(edge_ls_f: str) -> np.ndarray:
    pass

def load_vertex_rank(rank_f: str):
    pass

def load_coords(coords_ls: str) -> dict:
    with open(coords_ls, 'r') as f:
        content = f.readlines()

    coords_dict = {}
    curr_roi = ""
    for ln in content:
        curr_ln = ln[:-1].split(',')
        if (len(curr_ln) == 2): # the ROI line
            coords_dict[curr_ln[0]] = []
            curr_roi = curr_ln[0]
        else:
            coords_dict[curr_roi].append([float(c) for c in curr_ln])


    return coords_dict


def plot_con_mat(adj_mat, coords: dict):
    labels = []
    for key in coords.keys():
        labels += [key for _ in range(len(coords[key]))]

    plotting.plot_matrix(adj_matrix, figure=(10, 8), labels=labels[1:],
                         vmax=0.8, vmin=-0.8, reorder=True)


def plot_vertices_brain(rank_scores, coords:dict, ):
    # plot vertices with different ranks
    plotting.plot_markers(rank_scores, coords, title="", node_cmap=cm.YlOrRd)
    view.open_in_browser()


def plot_con_brain_2D(adj_mat, coords: dict):
    # plot flat brain connectome
    plotting.plot_connectome(adj_mat, coords,
                             title="",
                             display_mode='lyrz')

    plotting.show()


def plot_con_brain_3D(adj_mat, coords:dict):
    # plot interactive 3D brain
    plotting.view_connectome(adj_mat, coords, edge_threshold='90%')
    view.open_in_browser()


def main():

    ##=================== load data
    edge_ls_f = ""
    rank_f = ""
    coords_ls = "../data/coords.txt"

    # things needed:
    adj_mat = load_adjcency_mat(edge_ls_f) # the adjacency matrix
    rank_f = load_vertex_rank(rank_f) # an array of rank scores corresponding to the vertices
    coords = load_coords(coords_ls) # the coordinates of all vertices, can be from the original coords.txt

    ##==================== plot stuff
    plot_con_mat(adj_mat, coords)


if __name__ == "__main__":
    main()
