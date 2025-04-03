#ifndef REGISTRO
#define REGISTRO

template<class T>
struct Registro{
    int clave = 0;
    T datos;
    int dir;
};
#endif // REGISTRO