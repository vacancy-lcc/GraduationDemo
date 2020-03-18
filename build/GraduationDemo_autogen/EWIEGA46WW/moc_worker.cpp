/****************************************************************************
** Meta object code from reading C++ file 'worker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../mnt/hgfs/share/project/RFIDDemo/worker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'worker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_videoworker_t {
    QByteArrayData data[9];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_videoworker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_videoworker_t qt_meta_stringdata_videoworker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "videoworker"
QT_MOC_LITERAL(1, 12, 11), // "sendjpgpath"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 5), // "char*"
QT_MOC_LITERAL(4, 31, 4), // "path"
QT_MOC_LITERAL(5, 36, 6), // "_carid"
QT_MOC_LITERAL(6, 43, 10), // "reco_start"
QT_MOC_LITERAL(7, 54, 10), // "begin_reco"
QT_MOC_LITERAL(8, 65, 12) // "readJsonSlot"

    },
    "videoworker\0sendjpgpath\0\0char*\0path\0"
    "_carid\0reco_start\0begin_reco\0readJsonSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_videoworker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       6,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   40,    2, 0x0a /* Public */,
       8,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void videoworker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        videoworker *_t = static_cast<videoworker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendjpgpath((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->reco_start(); break;
        case 2: _t->begin_reco(); break;
        case 3: _t->readJsonSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (videoworker::*_t)(char * , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&videoworker::sendjpgpath)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (videoworker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&videoworker::reco_start)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject videoworker::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_videoworker.data,
      qt_meta_data_videoworker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *videoworker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *videoworker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_videoworker.stringdata0))
        return static_cast<void*>(const_cast< videoworker*>(this));
    return QThread::qt_metacast(_clname);
}

int videoworker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void videoworker::sendjpgpath(char * _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void videoworker::reco_start()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
struct qt_meta_stringdata_rfidWorker_t {
    QByteArrayData data[7];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_rfidWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_rfidWorker_t qt_meta_stringdata_rfidWorker = {
    {
QT_MOC_LITERAL(0, 0, 10), // "rfidWorker"
QT_MOC_LITERAL(1, 11, 8), // "flushSql"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 8), // "sendcash"
QT_MOC_LITERAL(4, 30, 4), // "cash"
QT_MOC_LITERAL(5, 35, 10), // "sendcardid"
QT_MOC_LITERAL(6, 46, 2) // "id"

    },
    "rfidWorker\0flushSql\0\0sendcash\0cash\0"
    "sendcardid\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_rfidWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,
       5,    1,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void rfidWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        rfidWorker *_t = static_cast<rfidWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->flushSql(); break;
        case 1: _t->sendcash((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sendcardid((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (rfidWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rfidWorker::flushSql)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (rfidWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rfidWorker::sendcash)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (rfidWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&rfidWorker::sendcardid)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject rfidWorker::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_rfidWorker.data,
      qt_meta_data_rfidWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *rfidWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *rfidWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_rfidWorker.stringdata0))
        return static_cast<void*>(const_cast< rfidWorker*>(this));
    return QThread::qt_metacast(_clname);
}

int rfidWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void rfidWorker::flushSql()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void rfidWorker::sendcash(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void rfidWorker::sendcardid(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
