#ifndef REGISTRO
#define REGISTRO

template<typename T>
struct Registro{
    int clave = 0;
    T datos;
    int dir;
};
#endif // REGISTRO