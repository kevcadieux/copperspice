/***********************************************************************
*
* Copyright (c) 2012-2020 Barbara Geller
* Copyright (c) 2012-2020 Ansel Sermersheim
*
* Copyright (c) 2015 The Qt Company Ltd.
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://www.gnu.org/licenses/
*
***********************************************************************/

// do not move include, if qvarient.h is included directly forward declarations are not sufficient 12/30/2013
#include <qobject.h>

#ifndef QVARIANT_H
#define QVARIANT_H

#include <csmetafwd.h>

#include <qatomic.h>
#include <qbytearray.h>
#include <qlist.h>
#include <qmap.h>
#include <qmetatype.h>
#include <qnamespace.h>
#include <qvector.h>
#include <qcontainerfwd.h>

// can not include qstring.h since it includes qstringparser.h, which then includes qlocale.h (circular dependency)
#include <qstring8.h>

class QBitArray;
class QDataStream;
class QDate;
class QDateTime;
class QDebug;
class QEasingCurve;
class QLine;
class QLineF;
class QLocale;
class QMatrix;
class QTransform;
class QStringList;
class QTime;
class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;
class QTextFormat;
class QTextLength;
class QUrl;
class QVariant;
class QVariantComparisonHelper;

template<typename T>
inline T qvariant_cast(const QVariant &);

class Q_CORE_EXPORT QVariant
{
 public:

   enum Type {
      Invalid,

      Bool,
      Short,
      UShort,
      Int,
      UInt,
      Long,
      ULong,
      LongLong,
      ULongLong,
      Double,
      Float,

      QChar,
      Char,
      SChar,
      UChar,
      Char8_t,
      Char16_t,
      Char32_t,

      ByteArray,
      BitArray,
      String,
      String8       = String,
      String16,
      StringList,
      StringView,
      RegularExpression,

      Date,
      Time,
      DateTime,
      Locale,

      JsonValue,
      JsonArray,
      JsonObject,
      JsonDocument,

      Line,
      LineF,
      Point,
      PointF,
      Polygon,
      PolygonF,
      Rect,
      RectF,
      Size,
      SizeF,

      Variant,
      List,
      Map,
      Hash,
      MultiMap,
      MultiHash,

      Void,
      VoidStar,
      ObjectStar,
      WidgetStar,

      // core
      EasingCurve,
      Margins,
      ModelIndex,
      PersistentModelIndex,
      Url,
      Uuid,

      // gui
      Bitmap,
      Brush,
      Color,
      Cursor,
      Font,
      Icon,
      Image,
      KeySequence,
      Matrix,
      Matrix4x4,
      Palette,
      Pen,
      Pixmap,
      Quaternion,
      Region,
      SizePolicy,
      TextLength,
      TextFormat,
      Transform,
      Vector2D,
      Vector3D,
      Vector4D,


      // temporary code, delete after transistion
      Char32               = QChar,
      FirstConstructedType = QChar,


      // must always be after all declared types
      UserType   = 256,

      // force compiler to allocate 32 bits
      LastType   = 0xffffffff
   };

   struct NamesAndTypes {
      const char *meta_typeName;
      uint meta_typeId;
      std::type_index meta_typeT;
   };
   QVariant();
   ~QVariant();

   // force compile error, prevent QVariant(bool) to be called
   QVariant(const char *) = delete;
   QVariant(void *) = delete;

   QVariant(Type type);
   QVariant(int typeOrUserType, const void *copy);
   QVariant(int typeOrUserType, const void *copy, uint flags);
   QVariant(const QVariant &other);

   QVariant(QDataStream &stream);

   QVariant(int value);
   QVariant(uint value);
   QVariant(qint64 value);
   QVariant(quint64 value);
   QVariant(bool value);
   QVariant(double value);

   QVariant(float value) {
      d.is_null = false;
      d.type = QMetaType::Float;
      d.data.f = value;
   }

   QVariant(const QByteArray &bytearray);
   QVariant(const QBitArray &bitarray);

   QVariant(const QChar32 &ch);
   QVariant(const QString8 &string);
   QVariant(const QString16 &string);

   QVariant(const QRegularExpression8 &regExp);

   QVariant(const QDate &date);
   QVariant(const QTime &time);
   QVariant(const QDateTime &datetime);

   QVariant(const QList<QVariant> &list);
   QVariant(const QStringList &stringList);

   QVariant(const QMap<QString, QVariant> &map);
   QVariant(const QHash<QString, QVariant> &hash);
   QVariant(const QMultiMap<QString, QVariant> &map);
   QVariant(const QMultiHash<QString, QVariant> &hash);

   QVariant(const QSize &size);
   QVariant(const QSizeF &size);
   QVariant(const QPoint &point);
   QVariant(const QPointF &point);
   QVariant(const QLine &line);
   QVariant(const QLineF &line);
   QVariant(const QRect &rect);
   QVariant(const QRectF &rect);

   QVariant(const QLocale &locale);

   QVariant(const QUrl &url);
   QVariant(const QEasingCurve &easing);
   QVariant(const QUuid &uuid);
   QVariant(const QModelIndex &modelIndex);
   QVariant(const QJsonValue &jsonValue);
   QVariant(const QJsonObject &jsonObject);
   QVariant(const QJsonArray &jsonArray);
   QVariant(const QJsonDocument &jsonDocument);

   QVariant &operator=(const QVariant &other);
   QVariant(Qt::GlobalColor color);

   inline QVariant &operator=(QVariant && other) {
      qSwap(d, other.d);
      return *this;
   }

   inline void swap(QVariant &other) {
      qSwap(d, other.d);
   }

   Type type() const;
   int userType() const;
   const QString &typeName() const;

   bool canConvert(Type t) const;
   bool convert(Type t);

   inline bool isValid() const;
   bool isNull() const;

   void clear();

   void detach();
   inline bool isDetached() const;

   int toInt(bool *ok = 0) const;
   uint toUInt(bool *ok = 0) const;
   qint64 toLongLong(bool *ok = 0) const;
   quint64 toULongLong(bool *ok = 0) const;
   bool toBool() const;
   double toDouble(bool *ok = 0) const;
   float toFloat(bool *ok = 0) const;
   qreal toReal(bool *ok = 0) const;

   QByteArray toByteArray() const;
   QBitArray toBitArray() const;

   QChar32 toChar() const;
   QString8 toString() const;
   QString16 toString16() const;

   QList<QVariant> toList() const;
   QStringList toStringList() const;

   QRegularExpression8 toRegularExpression() const;

   QDate toDate() const;
   QTime toTime() const;
   QDateTime toDateTime() const;

   QMap<QString, QVariant> toMap() const;
   QHash<QString, QVariant> toHash() const;
   QMultiMap<QString, QVariant> toMultiMap() const;
   QMultiHash<QString, QVariant> toMultiHash() const;

   QPoint toPoint() const;
   QPointF toPointF() const;
   QRect toRect() const;
   QSize toSize() const;
   QSizeF toSizeF() const;
   QLine toLine() const;
   QLineF toLineF() const;
   QRectF toRectF() const;

   QLocale toLocale() const;
   QUrl toUrl() const;
   QEasingCurve toEasingCurve() const;
   QUuid toUuid() const;
   QModelIndex toModelIndex() const;
   QJsonValue toJsonValue() const;
   QJsonObject toJsonObject() const;
   QJsonArray toJsonArray() const;
   QJsonDocument toJsonDocument() const;

   template<typename T>
   T value() const;
   void load(QDataStream &ds);
   void save(QDataStream &ds) const;

   static const QString &typeToName(Type type);
   static Type nameToType(const QString &name);

   void *data();
   const void *constData() const;

   inline const void *data() const {
      return constData();
   }

   template<typename T>
   inline void setValue(const T &value);

   inline void setValue(const QVariant &value);


   template<typename T>
   static inline QVariant fromValue(const T &value) {
      return QVariant(qMetaTypeId<T>(static_cast<T *>(nullptr)), &value, std::is_pointer_v<T>);
   }

   static inline QVariant fromValue(const QVariant &value) {
      return value;
   }

   template<typename T>
   bool canConvert() const {
      return canConvert(Type(qMetaTypeId<T>()));
   }
   template<typename T>
   static uint typeToTypeId()
   {
      // typeid() part of RTTI, core language
      uint retval = QVariant::getTypeId(typeid(T *));

      if (retval == QVariant::Invalid) {
         // T is a user defined data type

         // auto register and generate a type id for the given T
         retval = QVariant::registerType<T>();
      }

      return retval;
   };

   //
   struct PrivateShared {
      inline PrivateShared(void *v) : ptr(v), ref(1) { }
      void *ptr;
      QAtomicInt ref;
   };

   struct Private {
      inline Private(): type(Invalid), is_shared(false), is_null(true), is_ptr(false) {
         data.ptr = 0;
      }

      inline Private(const Private &other)
         : data(other.data), type(other.type), is_shared(other.is_shared), is_null(other.is_null), is_ptr(other.is_ptr) {
      }

      union Data {
         char c;
         int i;
         uint u;
         bool b;
         double d;
         float f;
         qreal real;
         qint64 ll;
         quint64 ull;
         QObject *o;
         void *ptr;
         PrivateShared *shared;
      } data;

      uint type      : 30;
      uint is_shared : 1;
      uint is_null   : 1;
      uint is_ptr    : 1;
   };

   typedef void (*f_construct)(Private *, const void *);
   typedef void (*f_clear)(Private *);
   typedef bool (*f_null)(const Private *);

   typedef void (*f_load)(Private *, QDataStream &);
   typedef void (*f_save)(const Private *, QDataStream &);

   typedef bool (*f_compare)(const Private *, const Private *);
   typedef bool (*f_convert)(const QVariant::Private *d, Type t, void *, bool *);
   typedef bool (*f_canConvert)(const QVariant::Private *d, Type t);
   typedef void (*f_debugStream)(QDebug, const QVariant &);

   struct Handler {
      f_construct construct;
      f_clear clear;
      f_null isNull;
      f_load load;
      f_save save;
      f_compare compare;
      f_convert convert;
      f_canConvert canConvert;
      f_debugStream debugStream;
   };

   inline bool operator==(const QVariant &other) const {
      return cmp(other);
   }

   inline bool operator!=(const QVariant &other) const {
      return ! cmp(other);
   }

   typedef Private DataPtr;
   inline DataPtr &data_ptr() {
      return d;
   }

 protected:
   friend inline bool qvariant_cast_helper(const QVariant &, QVariant::Type, void *);
   friend int qRegisterGuiVariant();
   friend int qUnregisterGuiVariant();
   friend inline bool operator==(const QVariant &, const QVariantComparisonHelper &);
   friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QVariant &);
   //
   template <typename T>
   static uint registerType();

   Private d;

   static const Handler *handler;

   void create(int type, const void *copy);
   bool cmp(const QVariant &other) const;

 private:
   bool clearRequired() const;

   static uint getTypeId(const std::type_index &index);
   static uint getTypeId(QString name);

   static std::atomic<uint> &currentUserType();

   static QVector<NamesAndTypes> m_userTypes;
   // force compile error, prevent QVariant(QVariant::Type, int) to be called
   QVariant(bool, int) = delete;
};

typedef QList<QVariant> QVariantList;

typedef QMap<QString, QVariant> QVariantMap;
typedef QHash<QString, QVariant> QVariantHash;
typedef QMultiMap<QString, QVariant> QVariantMultiMap;
typedef QMultiHash<QString, QVariant> QVariantMultiHash;

inline bool qvariant_cast_helper(const QVariant &v, QVariant::Type tp, void *ptr)
{
   return QVariant::handler->convert(&v.d, tp, ptr, 0);
}

template <typename T>
QVariant::Type qMetaTypeVariant(T * = nullptr)
{
   return static_cast<QVariant::Type>(QMetaTypeId2<T>::qt_metatype_id());
}

//
inline QVariant::QVariant() {}

inline bool QVariant::isValid() const
{
   return d.type != Invalid;
}

template<typename T>
inline void QVariant::setValue(const T &v)
{
   // optimize when std::any is added

   const uint type = qMetaTypeId<T>(static_cast<T *>(nullptr));
   *this = QVariant(type, &v, std::is_pointer_v<T>);
}

inline void QVariant::setValue(const QVariant &v)
{
   *this = v;
}

Q_CORE_EXPORT QDataStream &operator>> (QDataStream &s, QVariant &p);
Q_CORE_EXPORT QDataStream &operator<< (QDataStream &s, const QVariant &p);
Q_CORE_EXPORT QDataStream &operator>> (QDataStream &s, QVariant::Type &p);
Q_CORE_EXPORT QDataStream &operator<< (QDataStream &s, const QVariant::Type p);

inline bool QVariant::isDetached() const
{
   return !d.is_shared || d.data.shared->ref.load() == 1;
}


// Helper class to add one more level of indirection to prevent implicit casts
class QVariantComparisonHelper
{
 public:
   inline QVariantComparisonHelper(const QVariant &var)
      : v(&var) {}

 private:
   friend inline bool operator==(const QVariant &, const QVariantComparisonHelper &);
   const QVariant *v;
};

inline bool operator==(const QVariant &v1, const QVariantComparisonHelper &v2)
{
   return v1.cmp(*v2.v);
}

inline bool operator!=(const QVariant &v1, const QVariantComparisonHelper &v2)
{
   return !operator==(v1, v2);
}




template<typename T>
T QVariant::value() const
{
   const int id = qMetaTypeId<T>(static_cast<T *>(0));

   if (id == userType()) {
      return *reinterpret_cast<const T *>(this->constData());
   }

   if (id < int(QMetaType::User)) {
      T t;

      if (qvariant_cast_helper(*this, QVariant::Type(id), &t)) {
         return t;
      }
   }

   return T();
}


template<typename T>
T qvariant_cast(const QVariant &x)
{
   (void) x;
   static_assert(! std::is_same_v<T, T>, "qvariant_cast<T>(x) obsolete, use x.value<T>()");

   return T();
}

template <typename T>
uint QVariant::registerType()
{
   static std::atomic<uint> userId = QVariant::Invalid;

   if (userId.load(std::memory_order_relaxed) == QVariant::Invalid) {
      uint newId = QVariant::currentUserType().fetch_add(1, std::memory_order_relaxed);
      uint oldId = QVariant::Invalid;

      if (userId.compare_exchange_strong(oldId, newId, std::memory_order_release, std::memory_order_acquire))  {
         static QString typeName = cs_typeName<T>();
         m_userTypes.append(QVariant::NamesAndTypes{typeName.constData(), newId, typeid(T *)});

      } else {
         // already registered, maybe on a different thread
         return oldId;
      }
   }

   return userId.load(std::memory_order_acquire);
};

Q_DECLARE_SHARED(QVariant)

Q_CORE_EXPORT QDebug operator<<(QDebug, const QVariant &);
Q_CORE_EXPORT QDebug operator<<(QDebug, const QVariant::Type);

Q_DECLARE_BUILTIN_METATYPE(QVariantList, QVariantList)
Q_DECLARE_BUILTIN_METATYPE(QVariantMap,  QVariantMap)
Q_DECLARE_BUILTIN_METATYPE(QVariantHash, QVariantHash)

#endif
