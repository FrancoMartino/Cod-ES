#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>
#include <avr/eeprom.h>
#include <avr/io.h>

#if defined(__has_include) && __has_include(<type_traits>)
#include <type_traits>
#endif

#include <WString.h>    

struct EERef{

    EERef( const int index )
    : index( index )                 {}

    uint8_t operator*() const            { return eeprom_read_byte( (uint8_t*) index ); }
    operator const uint8_t() const       { return **this; }

    EERef &operator=( const EERef &ref ) { return *this = *ref; }
    EERef &operator=( uint8_t in )       { return eeprom_write_byte( (uint8_t*) index, in ), *this;  }
    EERef &operator +=( uint8_t in )     { return *this = **this + in; }
    EERef &operator -=( uint8_t in )     { return *this = **this - in; }
    EERef &operator *=( uint8_t in )     { return *this = **this * in; }
    EERef &operator /=( uint8_t in )     { return *this = **this / in; }
    EERef &operator ^=( uint8_t in )     { return *this = **this ^ in; }
    EERef &operator %=( uint8_t in )     { return *this = **this % in; }
    EERef &operator &=( uint8_t in )     { return *this = **this & in; }
    EERef &operator |=( uint8_t in )     { return *this = **this | in; }
    EERef &operator <<=( uint8_t in )    { return *this = **this << in; }
    EERef &operator >>=( uint8_t in )    { return *this = **this >> in; }

    EERef &update( uint8_t in )          { return  in != *this ? *this = in : *this; }

    EERef& operator++()                  { return *this += 1; }
    EERef& operator--()                  { return *this -= 1; }

    uint8_t operator++ (int) {
        uint8_t ret = **this;
        return ++(*this), ret;
    }

    uint8_t operator-- (int) {
        uint8_t ret = **this;
        return --(*this), ret;
    }

    int index; 
};

struct EEPtr{

    EEPtr( const int index )
    : index( index )                {}

    operator const int() const          { return index; }
    EEPtr &operator=( int in )          { return index = in, *this; }

    bool operator!=( const EEPtr &ptr ) { return index != ptr.index; }
    EERef operator*()                   { return index; }

    EEPtr& operator++()                 { return ++index, *this; }
    EEPtr& operator--()                 { return --index, *this; }
    EEPtr operator++ (int)              { return index++; }
    EEPtr operator-- (int)              { return index--; }

    int index; 
};

struct EEPROMClass{

#if defined(__arm__) && defined(TEENSYDUINO)
    EEPROMClass()                        { eeprom_initialize(); }
#endif

    EERef operator[]( const int idx )    { return idx; }
    uint8_t leer( int idx )              { return EERef( idx ); }
    void escribir( int idx, uint8_t val )   { (EERef( idx )) = val; }
    void actualizar( int idx, uint8_t val )  { EERef( idx ).update( val ); }

    EEPtr iniciar()                        { return 0x00; }
    EEPtr terminar()                          { return longitud(); } 
    uint16_t longitud()                    { return E2END + 1; }

    template< typename T > T &obtener( int idx, T &t ){
        #if defined(__has_include) && __has_include(<type_traits>)
    static_assert(std::is_trivially_copyable<T>::value,"You can not use this type with EEPROM.get" ); 
        #endif
    EEPtr e = idx;
    uint8_t *ptr = (uint8_t*) &t;
    for( int count = sizeof(T) ; count ; --count, ++e )  *ptr++ = *e;
        return t;
}

    template< typename T > const T &poner( int idx, const T &t ){
        #if defined(__has_include) && __has_include(<type_traits>)
static_assert(std::is_trivially_copyable<T>::value, "You can not use this type with EEPROM.put"); 
        #endif
const uint8_t *ptr = (const uint8_t*) &t;
#ifdef __arm__
eeprom_write_block(ptr, (void *)idx, sizeof(T));
#else
EEPtr e = idx;
for( int count = sizeof(T) ; count ; --count, ++e )  (*e).update( *ptr++ );
#endif
    return t;
}
};

template <>
inline const String &EEPROMClass::poner(int idx, const String &s)
{
    const uint8_t *ptr = (uint8_t *)s.c_str();

#ifdef __arm__
    eeprom_write_block(ptr, (void *)idx, s.length() + 1); 
#else
    EEPtr e = idx;
    for (int count = s.length() + 1; count; --count, ++e)
        (*e).update(*ptr++);
#endif
    return s;
}

template <>
inline String &EEPROMClass::obtener(int idx, String &s){
    s = "";             
    EEPtr e = idx;

    char c = *e;        
    while (c != '\0')
    {
        s += c;
        c = *(++e);
    }
    return s;
}

static EEPROMClass EEPROM __attribute__ ((unused));
#endif