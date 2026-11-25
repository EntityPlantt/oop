#include <iostream>
#include <cmath>

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
constexpr float Q_rsqrt( float number )
{
	int32_t i;
	_Float32 x2, y;
	constexpr float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = std::bit_cast<int32_t>(y);                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = std::bit_cast<_Float32>(i);
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

struct ComplexNumber {
	float real, imag;

	ComplexNumber(float real = 0, float imag = 0): real(real), imag(imag) {}
	ComplexNumber(const ComplexNumber &z): real(z.real), imag(z.imag) {}
	ComplexNumber &operator=(const ComplexNumber &z) {
		real = z.real;
		imag = z.imag;
		return *this;
	}

	float lensq() const {
		return real * real + imag * imag;
	}
	// Модул
	float operator~() const {
		return 1 / Q_rsqrt(lensq());
	}
	ComplexNumber operator-() const {
		return {-real, -imag};
	}
	// Конјугирање
	ComplexNumber operator*() const {
		return {real, -imag};
	}
	ComplexNumber operator+(const ComplexNumber &z) const {
		return {real + z.real, imag + z.imag};
	}
	ComplexNumber operator-(const ComplexNumber &z) const {
		return {real - z.real, imag - z.imag};
	}
	ComplexNumber operator*(float x) const {
		return {real * x, imag * x};
	}
	ComplexNumber operator*(const ComplexNumber &z) const {
		return {real * z.real - imag * z.imag, imag * z.real + real * z.imag};
	}
	ComplexNumber operator/(float x) const {
		return {real / x, imag / x};
	}
	ComplexNumber operator/(const ComplexNumber &z) const {
		return *this * *z / z.lensq();
	}

	ComplexNumber &operator+=(const ComplexNumber &z) { return *this = *this + z; }
	ComplexNumber &operator-=(const ComplexNumber &z) { return *this = *this - z; }
	ComplexNumber &operator*=(const float &z) { return *this = *this * z; }
	ComplexNumber &operator*=(const ComplexNumber &z) { return *this = *this * z; }
	ComplexNumber &operator/=(const float &z) { return *this = *this / z; }
	ComplexNumber &operator/=(const ComplexNumber &z) { return *this = *this / z; }

	bool operator==(const ComplexNumber &z) const {
		return real == z.real && imag == z.imag;
	}
	bool operator!=(const ComplexNumber &z) const {
		return !(*this == z);
	}
	// За користење во сортирачки структури!
	bool operator<(const ComplexNumber &z) const {
		if (real == z.real) return imag < z.imag;
		return real < z.real;
	}

	ComplexNumber &operator++() {
		++real;
		return *this;
	}
	ComplexNumber operator++(int) {
		return ++*this;
	}
	ComplexNumber &operator--() {
		--real;
		return *this;
	}
	ComplexNumber operator--(int) {
		return --*this;
	}

	friend std::ostream &operator<<(std::ostream &os, const ComplexNumber &z) {
		return os << z.real << (z.imag < 0 ? " - " : " + ") << fabs(z.imag) << 'i';
	}
	friend std::istream &operator>>(std::istream &is, ComplexNumber &z) {
		return is >> z.real >> z.imag;
	}
};

int main() {
	ComplexNumber a(1, 1);
	std::cout << "sqrt(2) = " << ~a << '\n';
	std::cout << "a + 1 = " << a + 1 << '\n';
	std::cout << "sqrt(3) = " << ~(a + 1) << '\n';
	std::cout << "nekojsi broj = " << std::fixed << (ComplexNumber(rand(), rand()) / 1000) << '\n';
	a++;
}
