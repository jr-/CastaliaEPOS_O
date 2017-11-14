// EPOS Elliptic Curve Diffie-Hellman (ECDH) Component Implementation

#include <diffie_hellman.h>

__BEGIN_SYS

// Class attributes
//TODO: base point is dependent of SECRET_SIZE
const unsigned char Diffie_Hellman::_default_base_point_x[SECRET_SIZE] =
{
    (unsigned char)'\x86', (unsigned char)'\x5B', (unsigned char)'\x2C', (unsigned char)'\xA5',
    (unsigned char)'\x7C', (unsigned char)'\x60', (unsigned char)'\x28', (unsigned char)'\x0C',
    (unsigned char)'\x2D', (unsigned char)'\x9B', (unsigned char)'\x89', (unsigned char)'\x8B',
    (unsigned char)'\x52', (unsigned char)'\xF7', (unsigned char)'\x1F', (unsigned char)'\x16'
};

const unsigned char Diffie_Hellman::_default_base_point_y[SECRET_SIZE] =
{
    (unsigned char)'\x83', (unsigned char)'\x7A', (unsigned char)'\xED', (unsigned char)'\xDD',
    (unsigned char)'\x92', (unsigned char)'\xA2', (unsigned char)'\x2D', (unsigned char)'\xC0',
    (unsigned char)'\x13', (unsigned char)'\xEB', (unsigned char)'\xAF', (unsigned char)'\x5B',
    (unsigned char)'\x39', (unsigned char)'\xC8', (unsigned char)'\x5A', (unsigned char)'\xCF'
};


// Class methods
Diffie_Hellman::Diffie_Hellman(const Elliptic_Curve_Point & base_point) : _base_point(base_point)
{
    generate_keypair();
}

Diffie_Hellman::Diffie_Hellman()
{
    new (&_base_point.x) Bignum(_default_base_point_x, SECRET_SIZE);
    new (&_base_point.y) Bignum(_default_base_point_y, SECRET_SIZE);
    _base_point.z = 1;
    generate_keypair();
}

Diffie_Hellman::Shared_Key Diffie_Hellman::shared_key(Elliptic_Curve_Point public_key)
{
    db<Diffie_Hellman>(TRC) << "Diffie_Hellman::shared_key(pub=" << public_key << ",priv=" << _private << ")" << endl;

    public_key *= _private;
    public_key.x ^= public_key.y;

    db<Diffie_Hellman>(INF) << "Diffie_Hellman: shared key = " << public_key.x << endl;
    return public_key.x;
}

void Diffie_Hellman::Elliptic_Curve_Point::operator*=(const Coordinate & b)
{
    // Finding last '1' bit of b
    static const unsigned int bits_in_digit = sizeof(Coordinate::Digit) * 8;
    Coordinate::Digit now;
    unsigned int b_len = sizeof(Coordinate) / sizeof(Coordinate::Digit);
    for(; (b_len > 1) && (b[b_len - 1] == 0); b_len--);
    if((b_len == 0) || (b[b_len - 1] == 0)) {
        x = 0;
        y = 0;
        z = 0;
        return;
    }

    now = b[b_len - 1];

    bool bin[bits_in_digit]; // Binary representation of 'now'
    unsigned int current_bit = bits_in_digit;

    Elliptic_Curve_Point pp(*this);

    for(int i = bits_in_digit - 1; i >= 0; i--) {
        if(now % 2)
            current_bit = i + 1;
        bin[i] = now % 2;
        now /= 2;
    }

    for(int i = b_len - 1; i >= 0; i--) {
        for(; current_bit < bits_in_digit; current_bit++) {
            jacobian_double();
            if(bin[current_bit])
                add_jacobian_affine(pp);
        }
        if(i > 0) {
            now = b[i-1];
            for(int j = bits_in_digit-1; j >= 0; j--) {
                bin[j] = now % 2;
                now /= 2;
            }
            current_bit = 0;
        }
    }

    Coordinate Z;
    z.invert();
    Z = z;
    Z *= z;

    x *= Z;
    Z *= z;

    y *= Z;
    z = 1;
}

void Diffie_Hellman::Elliptic_Curve_Point::jacobian_double()
{
    Coordinate B, C(x), aux(z);

    aux *= z; C -= aux;
    aux += x; C *= aux;
    C *= 3;

    z *= y; z *= 2;

    y *= y; B = y;

    y *= x; y *= 4;

    B *= B; B *= 8;

    x = C; x *= x;
    aux = y; aux *= 2;
    x -= aux;

    y -= x; y *= C;
    y -= B;
}

void Diffie_Hellman::Elliptic_Curve_Point::add_jacobian_affine(const Elliptic_Curve_Point &b)
{
    Coordinate A(z), B, C, X, Y, aux, aux2;

    A *= z;

    B = A;

    A *= b.x;

    B *= z; B *= b.y;

    C = A; C -= x;

    B -= y;

    X = B; X *= B;
    aux = C; aux *= C;

    Y = aux;

    aux2 = aux; aux *= C;
    aux2 *= 2; aux2 *= x;
    aux += aux2; X -= aux;

    aux = Y; Y *= x;
    Y -= X; Y *= B;
    aux *= y; aux *= C;
    Y -= aux;

    z *= C;

    x = X; y = Y;
}

__END_SYS
