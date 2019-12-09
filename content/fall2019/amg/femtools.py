import numpy as np

# Quadrature
def golubwelsch(n, lo=-1, hi=1, radau=False, lobatto=False):
    beta = .5 / np.sqrt(1-(2*(np.arange(1,n)))**(-2.)) # 3-term recurrence coeffs
    T = np.diag(beta,1) + np.diag(beta,-1);     # Jacobi matrix
    if radau:
        # Eq. 2.4 of Gautschi, Gauss–Radau formulae for Jacobi and Laguerre weight functions, 2000
        T[-1,-1] = 1 - 2*(n-1)**2 / (2*(n-1)*(2*(n-1)+1))
    if lobatto:
        # from Golub 1973, Some modified matrix eigenvalue problems, alpha=0
        I = np.eye(n-1)
        e = np.zeros(n-1); e[-1] = 1
        gamma = np.linalg.solve(T[:n-1,:n-1] - lo*I, e)
        mu = np.linalg.solve(T[:n-1,:n-1] - hi*I, e)
        y = np.linalg.solve(np.array([[1, -gamma[-1]], [1, -mu[-1]]]), [lo, hi])
        T[-1,-1] = y[0]
        T[-1,-2] = T[-2,-1] = np.sqrt(y[1])
    D, V = np.linalg.eigh(T);                      # Eigenvalue decomposition
    i = np.argsort(D)         # Legendre points
    x = D[i]
    w = 2*V[0,i]**2                   # Quadrature weights
    x = 0.5*((hi+lo) + (hi-lo)*x)
    w = 0.5*(hi-lo)*w
    return x, w

def fdvander(q, x, deriv=0):
    from fdtools import fdstencil
    B = np.zeros((len(q), len(x)))
    for i in range(len(q)):
        B[i] = fdstencil(q[i], x)[deriv]
    return B

def mass1(p):
    x = golubwelsch(p+1, lobatto=True)[0]
    q, w = golubwelsch(p+1)
    B = fdvander(q, x)
    return x,(B.T * w) @ B

def tri_quad4():
    q = np.array([[ -0.10810301816807,   -0.78379396366386  ],
                     [ -0.78379396366386,   -0.10810301816807  ],
                     [ -0.10810301816807,   -0.10810301816807  ],
                     [-0.816847572980458,   0.633695145960917  ],
                     [ 0.633695145960917,  -0.816847572980458  ],
                     [-0.816847572980458,  -0.816847572980458  ]])
    w = np.array([ 0.446763179356023,
                      0.446763179356023,
                      0.446763179356023,
                      0.219903487310644,
                      0.219903487310644,
                      0.219903487310644])
    return q, w

class fe2tri:
    def __init__(self, p):
        x1 = np.array([[-1, 1], [-1, -1], [1, -1]])
        x2 = np.array([[-1, 0], [0, -1], [0, 0]])
        if p == 1:
            x = x1
        elif p == 2:
            x = np.vstack([x1, x2])
        self.p = p
        self.xref = x
        self.q, self.w = tri_quad4() # Could use fewer points for p==1
        V, _ = self.prime(x)
        Vinv = np.linalg.inv(V)
        Bprime, Dprime = self.prime(self.q)
        self.B = Bprime @ Vinv
        self.D = Dprime @ Vinv

    def prime(self, x):
        V = np.ones((len(x), len(self.xref)))
        dV = np.zeros((len(x), 2, len(self.xref)))
        V[:,1] = x[:,0]
        V[:,2] = x[:,1]
        # dV[:,2*i] is derivative in x direction, dV[:,2*i+1] is in y-direction
        dV[:,0,1] = 1
        dV[:,1,2] = 1
        if self.p > 1:
            V[:,3] = x[:,0]**2
            V[:,4] = x[:,0]*x[:,1]
            V[:,5] = x[:,1]**2
            dV[:,0,3] = 2*x[:,0]
            dV[:,0,4] = x[:,1]
            dV[:,1,4] = x[:,0]
            dV[:,1,5] = 2*x[:,1]
        return V, dV
    
    def meshref(self):
        # Mesh for plotting on reference element
        x1 = np.linspace(-1, 1)
        xx, yy = np.meshgrid(x1, x1)
        for i,y in enumerate(yy):
            xx[i] = np.linspace(-1, -y[0])
        return np.vstack([xx.flatten(), yy.flatten()]).T
    
    def plot(self):
        import matplotlib.pyplot as plt
        plt.plot(self.xref[:,0], self.xref[:,1], 'o')
        plt.plot(self.q[:,0], self.q[:,1], 's')
        plt.triplot([-1, -1, 1], [1, -1, -1])
        
        X = self.meshref()
        Vinv = np.linalg.inv(self.prime(self.xref)[0])
        Bprime = self.prime(X)[0]
        B = Bprime @ Vinv
        plt.figure()
        for i, B_i in enumerate(B.T):
            from matplotlib import cm
            plt.subplot(len(B.T)//3, 3, i+1)
            plt.tricontourf(X[:,0], X[:,1], B_i, 30, cmap=cm.seismic, vmin=-1, vmax=1)
