import numpy as np


def check_collision(b1, b2):
    x1, y1, c1 = b1
    x2, y2, c2 = b2

    A = np.array([[x1, y1],
                  [x2, y2]])
    b = np.array([-c1, -c2])

    try:
        sol = np.linalg.solve(A, b)
        x = np.float64(round(sol[0], 2))
        y = np.float64(round(sol[1], 2))
        return x, y
    except np.linalg.LinAlgError:
        return None

def check_surface(p1, p2, p3):
    x1, y1, z1 = p1
    x2, y2, z2 = p2
    x3, y3, z3 = p3

    A = np.array([[x1, y1, 1],
                  [x2, y2, 1],
                  [x3, y3, 1]])
    b = np.array([z1, z2, z3])

    r_a = np.linalg.matrix_rank(A)
    r_aug = np.linalg.matrix_rank(np.column_stack((A, b)))

    if r_a != r_aug or r_a < 3:
        return None

    try:
        coef = np.linalg.solve(A, b)
        return np.round(coef, 2)
    except np.linalg.LinAlgError:
        return None

def check_rotation(vec, rad):
    x, y, z = vec

    c = np.cos(rad)
    s = np.sin(rad)

    rot_mat = np.array([[c, -s, 0],
                        [s, c, 0],
                        [0, 0, 1]])

    rot_vec = np.dot(rot_mat, vec)
    res = np.array([round(i, 2) for i in rot_vec])

    return res
