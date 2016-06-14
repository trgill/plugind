/*
 * Generated by gdbus-codegen 2.46.2. DO NOT EDIT.
 *
 * The license of this code is the same as for the source it was derived from.
 */

#ifndef __MULTIPATH_DBUS_GENERATED_H__
#define __MULTIPATH_DBUS_GENERATED_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for com.redhat.multipath.Manager */

#define MULTIPATH_TYPE_MANAGER (multipath_manager_get_type ())
#define MULTIPATH_MANAGER(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MANAGER, multipathManager))
#define MULTIPATH_IS_MANAGER(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MANAGER))
#define MULTIPATH_MANAGER_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), MULTIPATH_TYPE_MANAGER, multipathManagerIface))

struct _multipathManager;
typedef struct _multipathManager multipathManager;
typedef struct _multipathManagerIface multipathManagerIface;

struct _multipathManagerIface
{
  GTypeInterface parent_iface;

  gboolean (*handle_flush) (
    multipathManager *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_reload) (
    multipathManager *object,
    GDBusMethodInvocation *invocation);

};

GType multipath_manager_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *multipath_manager_interface_info (void);
guint multipath_manager_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void multipath_manager_complete_reload (
    multipathManager *object,
    GDBusMethodInvocation *invocation,
    gint response);

void multipath_manager_complete_flush (
    multipathManager *object,
    GDBusMethodInvocation *invocation,
    gint response);



/* D-Bus method calls: */
void multipath_manager_call_reload (
    multipathManager *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean multipath_manager_call_reload_finish (
    multipathManager *proxy,
    gint *out_response,
    GAsyncResult *res,
    GError **error);

gboolean multipath_manager_call_reload_sync (
    multipathManager *proxy,
    gint *out_response,
    GCancellable *cancellable,
    GError **error);

void multipath_manager_call_flush (
    multipathManager *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean multipath_manager_call_flush_finish (
    multipathManager *proxy,
    gint *out_response,
    GAsyncResult *res,
    GError **error);

gboolean multipath_manager_call_flush_sync (
    multipathManager *proxy,
    gint *out_response,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define MULTIPATH_TYPE_MANAGER_PROXY (multipath_manager_proxy_get_type ())
#define MULTIPATH_MANAGER_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MANAGER_PROXY, multipathManagerProxy))
#define MULTIPATH_MANAGER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_MANAGER_PROXY, multipathManagerProxyClass))
#define MULTIPATH_MANAGER_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_MANAGER_PROXY, multipathManagerProxyClass))
#define MULTIPATH_IS_MANAGER_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MANAGER_PROXY))
#define MULTIPATH_IS_MANAGER_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_MANAGER_PROXY))

typedef struct _multipathManagerProxy multipathManagerProxy;
typedef struct _multipathManagerProxyClass multipathManagerProxyClass;
typedef struct _multipathManagerProxyPrivate multipathManagerProxyPrivate;

struct _multipathManagerProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  multipathManagerProxyPrivate *priv;
};

struct _multipathManagerProxyClass
{
  GDBusProxyClass parent_class;
};

GType multipath_manager_proxy_get_type (void) G_GNUC_CONST;

void multipath_manager_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
multipathManager *multipath_manager_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
multipathManager *multipath_manager_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void multipath_manager_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
multipathManager *multipath_manager_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
multipathManager *multipath_manager_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define MULTIPATH_TYPE_MANAGER_SKELETON (multipath_manager_skeleton_get_type ())
#define MULTIPATH_MANAGER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MANAGER_SKELETON, multipathManagerSkeleton))
#define MULTIPATH_MANAGER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_MANAGER_SKELETON, multipathManagerSkeletonClass))
#define MULTIPATH_MANAGER_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_MANAGER_SKELETON, multipathManagerSkeletonClass))
#define MULTIPATH_IS_MANAGER_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MANAGER_SKELETON))
#define MULTIPATH_IS_MANAGER_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_MANAGER_SKELETON))

typedef struct _multipathManagerSkeleton multipathManagerSkeleton;
typedef struct _multipathManagerSkeletonClass multipathManagerSkeletonClass;
typedef struct _multipathManagerSkeletonPrivate multipathManagerSkeletonPrivate;

struct _multipathManagerSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  multipathManagerSkeletonPrivate *priv;
};

struct _multipathManagerSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType multipath_manager_skeleton_get_type (void) G_GNUC_CONST;

multipathManager *multipath_manager_skeleton_new (void);


/* ------------------------------------------------------------------------ */
/* Declarations for com.redhat.multipath.map */

#define MULTIPATH_TYPE_MAP (multipath_map_get_type ())
#define MULTIPATH_MAP(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MAP, multipathMap))
#define MULTIPATH_IS_MAP(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MAP))
#define MULTIPATH_MAP_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), MULTIPATH_TYPE_MAP, multipathMapIface))

struct _multipathMap;
typedef struct _multipathMap multipathMap;
typedef struct _multipathMapIface multipathMapIface;

struct _multipathMapIface
{
  GTypeInterface parent_iface;

  gint  (*get_bestpg) (multipathMap *object);

  const gchar * (*get_features) (multipathMap *object);

  gint  (*get_nextpg) (multipathMap *object);

  gint  (*get_no_path_retry) (multipathMap *object);

  guint  (*get_nr_active) (multipathMap *object);

  const gchar * (*get_pathgroups) (multipathMap *object);

  const gchar *const * (*get_paths) (multipathMap *object);

  guint  (*get_pg_policy) (multipathMap *object);

  guint  (*get_pgfailback) (multipathMap *object);

  gint  (*get_queuedio) (multipathMap *object);

  const gchar * (*get_selector) (multipathMap *object);

  guint  (*get_size) (multipathMap *object);

  const gchar * (*get_uuid) (multipathMap *object);

};

GType multipath_map_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *multipath_map_interface_info (void);
guint multipath_map_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus property accessors: */
const gchar *multipath_map_get_uuid (multipathMap *object);
gchar *multipath_map_dup_uuid (multipathMap *object);
void multipath_map_set_uuid (multipathMap *object, const gchar *value);

guint multipath_map_get_size (multipathMap *object);
void multipath_map_set_size (multipathMap *object, guint value);

const gchar *multipath_map_get_features (multipathMap *object);
gchar *multipath_map_dup_features (multipathMap *object);
void multipath_map_set_features (multipathMap *object, const gchar *value);

guint multipath_map_get_pgfailback (multipathMap *object);
void multipath_map_set_pgfailback (multipathMap *object, guint value);

guint multipath_map_get_pg_policy (multipathMap *object);
void multipath_map_set_pg_policy (multipathMap *object, guint value);

gint multipath_map_get_nextpg (multipathMap *object);
void multipath_map_set_nextpg (multipathMap *object, gint value);

gint multipath_map_get_bestpg (multipathMap *object);
void multipath_map_set_bestpg (multipathMap *object, gint value);

gint multipath_map_get_queuedio (multipathMap *object);
void multipath_map_set_queuedio (multipathMap *object, gint value);

gint multipath_map_get_no_path_retry (multipathMap *object);
void multipath_map_set_no_path_retry (multipathMap *object, gint value);

guint multipath_map_get_nr_active (multipathMap *object);
void multipath_map_set_nr_active (multipathMap *object, guint value);

const gchar *multipath_map_get_selector (multipathMap *object);
gchar *multipath_map_dup_selector (multipathMap *object);
void multipath_map_set_selector (multipathMap *object, const gchar *value);

const gchar *const *multipath_map_get_paths (multipathMap *object);
gchar **multipath_map_dup_paths (multipathMap *object);
void multipath_map_set_paths (multipathMap *object, const gchar *const *value);

const gchar *multipath_map_get_pathgroups (multipathMap *object);
gchar *multipath_map_dup_pathgroups (multipathMap *object);
void multipath_map_set_pathgroups (multipathMap *object, const gchar *value);


/* ---- */

#define MULTIPATH_TYPE_MAP_PROXY (multipath_map_proxy_get_type ())
#define MULTIPATH_MAP_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MAP_PROXY, multipathMapProxy))
#define MULTIPATH_MAP_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_MAP_PROXY, multipathMapProxyClass))
#define MULTIPATH_MAP_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_MAP_PROXY, multipathMapProxyClass))
#define MULTIPATH_IS_MAP_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MAP_PROXY))
#define MULTIPATH_IS_MAP_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_MAP_PROXY))

typedef struct _multipathMapProxy multipathMapProxy;
typedef struct _multipathMapProxyClass multipathMapProxyClass;
typedef struct _multipathMapProxyPrivate multipathMapProxyPrivate;

struct _multipathMapProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  multipathMapProxyPrivate *priv;
};

struct _multipathMapProxyClass
{
  GDBusProxyClass parent_class;
};

GType multipath_map_proxy_get_type (void) G_GNUC_CONST;

void multipath_map_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
multipathMap *multipath_map_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
multipathMap *multipath_map_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void multipath_map_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
multipathMap *multipath_map_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
multipathMap *multipath_map_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define MULTIPATH_TYPE_MAP_SKELETON (multipath_map_skeleton_get_type ())
#define MULTIPATH_MAP_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MAP_SKELETON, multipathMapSkeleton))
#define MULTIPATH_MAP_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_MAP_SKELETON, multipathMapSkeletonClass))
#define MULTIPATH_MAP_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_MAP_SKELETON, multipathMapSkeletonClass))
#define MULTIPATH_IS_MAP_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MAP_SKELETON))
#define MULTIPATH_IS_MAP_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_MAP_SKELETON))

typedef struct _multipathMapSkeleton multipathMapSkeleton;
typedef struct _multipathMapSkeletonClass multipathMapSkeletonClass;
typedef struct _multipathMapSkeletonPrivate multipathMapSkeletonPrivate;

struct _multipathMapSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  multipathMapSkeletonPrivate *priv;
};

struct _multipathMapSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType multipath_map_skeleton_get_type (void) G_GNUC_CONST;

multipathMap *multipath_map_skeleton_new (void);


/* ------------------------------------------------------------------------ */
/* Declarations for com.redhat.multipath.map.path */

#define MULTIPATH_TYPE_MAP_PATH (multipath_map_path_get_type ())
#define MULTIPATH_MAP_PATH(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MAP_PATH, multipathMapPath))
#define MULTIPATH_IS_MAP_PATH(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MAP_PATH))
#define MULTIPATH_MAP_PATH_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), MULTIPATH_TYPE_MAP_PATH, multipathMapPathIface))

struct _multipathMapPath;
typedef struct _multipathMapPath multipathMapPath;
typedef struct _multipathMapPathIface multipathMapPathIface;

struct _multipathMapPathIface
{
  GTypeInterface parent_iface;

  guint  (*get_bus) (multipathMapPath *object);

  const gchar * (*get_dev) (multipathMapPath *object);

  guint  (*get_dmstate) (multipathMapPath *object);

  guint  (*get_offline) (multipathMapPath *object);

  const gchar * (*get_product_id) (multipathMapPath *object);

  const gchar * (*get_rev) (multipathMapPath *object);

  const gchar * (*get_serial) (multipathMapPath *object);

  guint  (*get_state) (multipathMapPath *object);

  const gchar * (*get_tgt_node_name) (multipathMapPath *object);

  const gchar * (*get_vendor_id) (multipathMapPath *object);

};

GType multipath_map_path_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *multipath_map_path_interface_info (void);
guint multipath_map_path_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus property accessors: */
guint multipath_map_path_get_state (multipathMapPath *object);
void multipath_map_path_set_state (multipathMapPath *object, guint value);

guint multipath_map_path_get_dmstate (multipathMapPath *object);
void multipath_map_path_set_dmstate (multipathMapPath *object, guint value);

guint multipath_map_path_get_offline (multipathMapPath *object);
void multipath_map_path_set_offline (multipathMapPath *object, guint value);

guint multipath_map_path_get_bus (multipathMapPath *object);
void multipath_map_path_set_bus (multipathMapPath *object, guint value);

const gchar *multipath_map_path_get_dev (multipathMapPath *object);
gchar *multipath_map_path_dup_dev (multipathMapPath *object);
void multipath_map_path_set_dev (multipathMapPath *object, const gchar *value);

const gchar *multipath_map_path_get_vendor_id (multipathMapPath *object);
gchar *multipath_map_path_dup_vendor_id (multipathMapPath *object);
void multipath_map_path_set_vendor_id (multipathMapPath *object, const gchar *value);

const gchar *multipath_map_path_get_product_id (multipathMapPath *object);
gchar *multipath_map_path_dup_product_id (multipathMapPath *object);
void multipath_map_path_set_product_id (multipathMapPath *object, const gchar *value);

const gchar *multipath_map_path_get_rev (multipathMapPath *object);
gchar *multipath_map_path_dup_rev (multipathMapPath *object);
void multipath_map_path_set_rev (multipathMapPath *object, const gchar *value);

const gchar *multipath_map_path_get_serial (multipathMapPath *object);
gchar *multipath_map_path_dup_serial (multipathMapPath *object);
void multipath_map_path_set_serial (multipathMapPath *object, const gchar *value);

const gchar *multipath_map_path_get_tgt_node_name (multipathMapPath *object);
gchar *multipath_map_path_dup_tgt_node_name (multipathMapPath *object);
void multipath_map_path_set_tgt_node_name (multipathMapPath *object, const gchar *value);


/* ---- */

#define MULTIPATH_TYPE_MAP_PATH_PROXY (multipath_map_path_proxy_get_type ())
#define MULTIPATH_MAP_PATH_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MAP_PATH_PROXY, multipathMapPathProxy))
#define MULTIPATH_MAP_PATH_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_MAP_PATH_PROXY, multipathMapPathProxyClass))
#define MULTIPATH_MAP_PATH_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_MAP_PATH_PROXY, multipathMapPathProxyClass))
#define MULTIPATH_IS_MAP_PATH_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MAP_PATH_PROXY))
#define MULTIPATH_IS_MAP_PATH_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_MAP_PATH_PROXY))

typedef struct _multipathMapPathProxy multipathMapPathProxy;
typedef struct _multipathMapPathProxyClass multipathMapPathProxyClass;
typedef struct _multipathMapPathProxyPrivate multipathMapPathProxyPrivate;

struct _multipathMapPathProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  multipathMapPathProxyPrivate *priv;
};

struct _multipathMapPathProxyClass
{
  GDBusProxyClass parent_class;
};

GType multipath_map_path_proxy_get_type (void) G_GNUC_CONST;

void multipath_map_path_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
multipathMapPath *multipath_map_path_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
multipathMapPath *multipath_map_path_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void multipath_map_path_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
multipathMapPath *multipath_map_path_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
multipathMapPath *multipath_map_path_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define MULTIPATH_TYPE_MAP_PATH_SKELETON (multipath_map_path_skeleton_get_type ())
#define MULTIPATH_MAP_PATH_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_MAP_PATH_SKELETON, multipathMapPathSkeleton))
#define MULTIPATH_MAP_PATH_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_MAP_PATH_SKELETON, multipathMapPathSkeletonClass))
#define MULTIPATH_MAP_PATH_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_MAP_PATH_SKELETON, multipathMapPathSkeletonClass))
#define MULTIPATH_IS_MAP_PATH_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_MAP_PATH_SKELETON))
#define MULTIPATH_IS_MAP_PATH_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_MAP_PATH_SKELETON))

typedef struct _multipathMapPathSkeleton multipathMapPathSkeleton;
typedef struct _multipathMapPathSkeletonClass multipathMapPathSkeletonClass;
typedef struct _multipathMapPathSkeletonPrivate multipathMapPathSkeletonPrivate;

struct _multipathMapPathSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  multipathMapPathSkeletonPrivate *priv;
};

struct _multipathMapPathSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType multipath_map_path_skeleton_get_type (void) G_GNUC_CONST;

multipathMapPath *multipath_map_path_skeleton_new (void);


/* ---- */

#define MULTIPATH_TYPE_OBJECT (multipath_object_get_type ())
#define MULTIPATH_OBJECT(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_OBJECT, multipathObject))
#define MULTIPATH_IS_OBJECT(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_OBJECT))
#define MULTIPATH_OBJECT_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), MULTIPATH_TYPE_OBJECT, multipathObject))

struct _multipathObject;
typedef struct _multipathObject multipathObject;
typedef struct _multipathObjectIface multipathObjectIface;

struct _multipathObjectIface
{
  GTypeInterface parent_iface;
};

GType multipath_object_get_type (void) G_GNUC_CONST;

multipathManager *multipath_object_get_manager (multipathObject *object);
multipathMap *multipath_object_get_map (multipathObject *object);
multipathMapPath *multipath_object_get_map_path (multipathObject *object);
multipathManager *multipath_object_peek_manager (multipathObject *object);
multipathMap *multipath_object_peek_map (multipathObject *object);
multipathMapPath *multipath_object_peek_map_path (multipathObject *object);

#define MULTIPATH_TYPE_OBJECT_PROXY (multipath_object_proxy_get_type ())
#define MULTIPATH_OBJECT_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_OBJECT_PROXY, multipathObjectProxy))
#define MULTIPATH_OBJECT_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_OBJECT_PROXY, multipathObjectProxyClass))
#define MULTIPATH_OBJECT_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_OBJECT_PROXY, multipathObjectProxyClass))
#define MULTIPATH_IS_OBJECT_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_OBJECT_PROXY))
#define MULTIPATH_IS_OBJECT_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_OBJECT_PROXY))

typedef struct _multipathObjectProxy multipathObjectProxy;
typedef struct _multipathObjectProxyClass multipathObjectProxyClass;
typedef struct _multipathObjectProxyPrivate multipathObjectProxyPrivate;

struct _multipathObjectProxy
{
  /*< private >*/
  GDBusObjectProxy parent_instance;
  multipathObjectProxyPrivate *priv;
};

struct _multipathObjectProxyClass
{
  GDBusObjectProxyClass parent_class;
};

GType multipath_object_proxy_get_type (void) G_GNUC_CONST;
multipathObjectProxy *multipath_object_proxy_new (GDBusConnection *connection, const gchar *object_path);

#define MULTIPATH_TYPE_OBJECT_SKELETON (multipath_object_skeleton_get_type ())
#define MULTIPATH_OBJECT_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_OBJECT_SKELETON, multipathObjectSkeleton))
#define MULTIPATH_OBJECT_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_OBJECT_SKELETON, multipathObjectSkeletonClass))
#define MULTIPATH_OBJECT_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_OBJECT_SKELETON, multipathObjectSkeletonClass))
#define MULTIPATH_IS_OBJECT_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_OBJECT_SKELETON))
#define MULTIPATH_IS_OBJECT_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_OBJECT_SKELETON))

typedef struct _multipathObjectSkeleton multipathObjectSkeleton;
typedef struct _multipathObjectSkeletonClass multipathObjectSkeletonClass;
typedef struct _multipathObjectSkeletonPrivate multipathObjectSkeletonPrivate;

struct _multipathObjectSkeleton
{
  /*< private >*/
  GDBusObjectSkeleton parent_instance;
  multipathObjectSkeletonPrivate *priv;
};

struct _multipathObjectSkeletonClass
{
  GDBusObjectSkeletonClass parent_class;
};

GType multipath_object_skeleton_get_type (void) G_GNUC_CONST;
multipathObjectSkeleton *multipath_object_skeleton_new (const gchar *object_path);
void multipath_object_skeleton_set_manager (multipathObjectSkeleton *object, multipathManager *interface_);
void multipath_object_skeleton_set_map (multipathObjectSkeleton *object, multipathMap *interface_);
void multipath_object_skeleton_set_map_path (multipathObjectSkeleton *object, multipathMapPath *interface_);

/* ---- */

#define MULTIPATH_TYPE_OBJECT_MANAGER_CLIENT (multipath_object_manager_client_get_type ())
#define MULTIPATH_OBJECT_MANAGER_CLIENT(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), MULTIPATH_TYPE_OBJECT_MANAGER_CLIENT, multipathObjectManagerClient))
#define MULTIPATH_OBJECT_MANAGER_CLIENT_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), MULTIPATH_TYPE_OBJECT_MANAGER_CLIENT, multipathObjectManagerClientClass))
#define MULTIPATH_OBJECT_MANAGER_CLIENT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MULTIPATH_TYPE_OBJECT_MANAGER_CLIENT, multipathObjectManagerClientClass))
#define MULTIPATH_IS_OBJECT_MANAGER_CLIENT(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), MULTIPATH_TYPE_OBJECT_MANAGER_CLIENT))
#define MULTIPATH_IS_OBJECT_MANAGER_CLIENT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), MULTIPATH_TYPE_OBJECT_MANAGER_CLIENT))

typedef struct _multipathObjectManagerClient multipathObjectManagerClient;
typedef struct _multipathObjectManagerClientClass multipathObjectManagerClientClass;
typedef struct _multipathObjectManagerClientPrivate multipathObjectManagerClientPrivate;

struct _multipathObjectManagerClient
{
  /*< private >*/
  GDBusObjectManagerClient parent_instance;
  multipathObjectManagerClientPrivate *priv;
};

struct _multipathObjectManagerClientClass
{
  GDBusObjectManagerClientClass parent_class;
};

GType multipath_object_manager_client_get_type (void) G_GNUC_CONST;

GType multipath_object_manager_client_get_proxy_type (GDBusObjectManagerClient *manager, const gchar *object_path, const gchar *interface_name, gpointer user_data);

void multipath_object_manager_client_new (
    GDBusConnection        *connection,
    GDBusObjectManagerClientFlags  flags,
    const gchar            *name,
    const gchar            *object_path,
    GCancellable           *cancellable,
    GAsyncReadyCallback     callback,
    gpointer                user_data);
GDBusObjectManager *multipath_object_manager_client_new_finish (
    GAsyncResult        *res,
    GError             **error);
GDBusObjectManager *multipath_object_manager_client_new_sync (
    GDBusConnection        *connection,
    GDBusObjectManagerClientFlags  flags,
    const gchar            *name,
    const gchar            *object_path,
    GCancellable           *cancellable,
    GError                **error);

void multipath_object_manager_client_new_for_bus (
    GBusType                bus_type,
    GDBusObjectManagerClientFlags  flags,
    const gchar            *name,
    const gchar            *object_path,
    GCancellable           *cancellable,
    GAsyncReadyCallback     callback,
    gpointer                user_data);
GDBusObjectManager *multipath_object_manager_client_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
GDBusObjectManager *multipath_object_manager_client_new_for_bus_sync (
    GBusType                bus_type,
    GDBusObjectManagerClientFlags  flags,
    const gchar            *name,
    const gchar            *object_path,
    GCancellable           *cancellable,
    GError                **error);


G_END_DECLS

#endif /* __MULTIPATH_DBUS_GENERATED_H__ */
