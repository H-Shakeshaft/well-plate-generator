!!! abstract 

    Explanation of how the numbers are actually generated whilst simulating well plates.

The application simulates reads according to a sigmoidal curve function such that a well with $x$ with some concentration
$$
f(x) = \frac{1}{1 + e^{-x}}
$$ 

Internally this is represented as 
$$
f(x) = \frac{A}{B + e^{-x}}, A \in \mathbb{R}, B \in \mathbb{R}
$$

Currently this is 
$$
A = 1, B = 1
$$

<br/>

After $f(x)$ is applied, this value is taken and 'fuzzed' by adding a value generated according to a normal distribution
such that 
$$
g(x) = f(x) + \chi 
$$

where 
$$
\chi \in \mathcal{N}(\mu, \sigma^{2}), \mu = 0.5, \sigma = 0.3
$$
