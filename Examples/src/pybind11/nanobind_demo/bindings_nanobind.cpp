#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <Eigen/Dense>
#include <algorithm>
#include <cmath>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

namespace nb = nanobind;
using namespace nb::literals;

using Vector = Eigen::VectorXd;
using Matrix = Eigen::MatrixXd;

int add(int a, int b = 1) { return a + b; }

std::vector<double> scale(std::vector<double> x, double a) {
    for (auto &v : x) v *= a;
    return x;
}

std::map<std::string, double> moments(const std::vector<double> &x) {
    if (x.empty()) return {{"mean", 0.0}, {"variance", 0.0}};
    double sum = std::accumulate(x.begin(), x.end(), 0.0);
    double mean = sum / static_cast<double>(x.size());
    double sq = 0.0;
    for (auto v : x) sq += (v - mean) * (v - mean);
    return {{"mean", mean}, {"variance", sq / static_cast<double>(x.size())}};
}

Matrix rank_one(Vector x) {
    return x * x.transpose();
}

double energy(Eigen::Ref<const Vector> u, Eigen::Ref<const Matrix> A) {
    return u.dot(A * u);
}

void axpy(double a, Eigen::Ref<const Vector> x, Eigen::Ref<Vector> y) {
    y += a * x;
}

using Array1D = nb::ndarray<const double, nb::ndim<1>, nb::c_contig, nb::device::cpu>;

double sum_array(Array1D a) {
    double s = 0.0;
    for (size_t i = 0; i < a.shape(0); ++i) s += a(i);
    return s;
}

class Polynomial {
public:
    explicit Polynomial(std::vector<double> c) : c_(std::move(c)) {
        if (c_.empty()) c_.push_back(0.0);
    }

    std::size_t degree() const { return c_.empty() ? 0 : c_.size() - 1; }

    double operator()(double x) const {
        double y = 0.0;
        for (auto it = c_.rbegin(); it != c_.rend(); ++it) y = y * x + *it;
        return y;
    }

    const std::vector<double> &coefficients() const { return c_; }

private:
    std::vector<double> c_;
};

struct Point {
    double x = 0.0;
    double y = 0.0;
    double norm() const { return std::hypot(x, y); }
};

Point add_points(const Point &a, const Point &b) {
    return {a.x + b.x, a.y + b.y};
}

class State {
public:
    explicit State(int n) : u_(Vector::Zero(n)) {}
    Vector &values() { return u_; }

private:
    Vector u_;
};

template <class T>
T square(T x) { return x * x; }

template <class Scalar>
class Affine {
public:
    Affine(Scalar a, Scalar b) : a_(a), b_(b) {}
    Scalar operator()(Scalar x) const { return a_ * x + b_; }
    Scalar a() const { return a_; }
    Scalar b() const { return b_; }

private:
    Scalar a_, b_;
};

template <class Scalar>
void bind_affine(nb::module_ &m, const char *name) {
    using A = Affine<Scalar>;
    nb::class_<A>(m, name)
        .def(nb::init<Scalar, Scalar>(), "a"_a, "b"_a)
        .def("__call__", &A::operator(), "x"_a)
        .def_prop_ro("a", &A::a)
        .def_prop_ro("b", &A::b);
}

NB_MODULE(pacs_nb, m) {
    m.doc() = "PACS nanobind examples";

    m.def("add", &add, "a"_a, "b"_a = 1,
          "Return a+b; if b is omitted, increment a.");
    m.def("scale", &scale, "x"_a, "a"_a);
    m.def("moments", &moments, "x"_a);

    m.def("rank_one", &rank_one, "x"_a);
    m.def("energy", &energy, "u"_a, "A"_a);
    m.def("axpy", &axpy, "a"_a, "x"_a, "y"_a);
    m.def("sum_array", &sum_array, "a"_a);

    nb::class_<Polynomial>(m, "Polynomial")
        .def(nb::init<std::vector<double>>(), "c"_a)
        .def("degree", &Polynomial::degree)
        .def("__call__", &Polynomial::operator(), "x"_a)
        .def_prop_ro("coefficients",
            [](const Polynomial &p) { return p.coefficients(); })
        .def("__repr__", [](const Polynomial &p) {
            return "<Polynomial degree=" + std::to_string(p.degree()) + ">";
        });

    nb::class_<Point>(m, "Point")
        .def(nb::init<>())
        .def(nb::init<double, double>(), "x"_a, "y"_a)
        .def_rw("x", &Point::x)
        .def_rw("y", &Point::y)
        .def_prop_ro("norm", &Point::norm)
        .def("__add__", &add_points)
        .def("__repr__", [](const Point &p) {
            return "Point(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
        });

    nb::class_<State>(m, "State")
        .def(nb::init<int>(), "n"_a)
        .def("values", &State::values, nb::rv_policy::reference_internal);

    m.def("square_int", &square<int>);
    m.def("square_double", &square<double>);
    bind_affine<double>(m, "AffineD");
    bind_affine<float>(m, "AffineF");
}
