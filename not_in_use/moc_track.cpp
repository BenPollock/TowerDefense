/****************************************************************************
** Meta object code from reading C++ file 'track.h'
**
** Created: Wed Nov 16 18:44:52 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tower_defense/track.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'track.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Track[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    7,    6,    6, 0x0a,
      26,    6,    6,    6, 0x0a,
      37,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Track[] = {
    "Track\0\0t\0selectTower(int)\0gameFlow()\0"
    "loadMap()\0"
};

const QMetaObject Track::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Track,
      qt_meta_data_Track, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Track::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Track::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Track::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Track))
        return static_cast<void*>(const_cast< Track*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< Track*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< Track*>(this));
    return QObject::qt_metacast(_clname);
}

int Track::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectTower((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: gameFlow(); break;
        case 2: loadMap(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
