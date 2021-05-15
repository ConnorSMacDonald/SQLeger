#ifndef SQLEGER_CONSTANTS_HPP
#define SQLEGER_CONSTANTS_HPP

#include <sqleger/string.hpp>

#include <sqlite3.h>

#include <algorithm>
#include <type_traits>


namespace sqleger {


template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
constexpr std::underlying_type_t<Enum> enum_to_int(Enum value) noexcept;

template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
constexpr Enum int_to_enum(std::underlying_type_t<Enum> value) noexcept;


enum class result : int {
  // Primary result codes
  ok = SQLITE_OK,
  error = SQLITE_ERROR,
  internal = SQLITE_INTERNAL,
  perm = SQLITE_PERM,
  abort = SQLITE_ABORT,
  busy = SQLITE_BUSY,
  locked = SQLITE_LOCKED,
  nomem = SQLITE_NOMEM,
  readonly = SQLITE_READONLY,
  interrupt = SQLITE_INTERRUPT,
  ioerr = SQLITE_IOERR,
  corrupt = SQLITE_CORRUPT,
  notfound = SQLITE_NOTFOUND,
  full = SQLITE_FULL,
  cantopen = SQLITE_CANTOPEN,
  protocol = SQLITE_PROTOCOL,
  schema = SQLITE_SCHEMA,
  toobig = SQLITE_TOOBIG,
  constraint = SQLITE_CONSTRAINT,
  mismatch = SQLITE_MISMATCH,
  misuse = SQLITE_MISUSE,
  nolfs = SQLITE_NOLFS,
  auth = SQLITE_AUTH,
  range = SQLITE_RANGE,
  notadb = SQLITE_NOTADB,
  notice = SQLITE_NOTICE,
  warning = SQLITE_WARNING,
  row = SQLITE_ROW,
  done = SQLITE_DONE,
  // Extended result codes
  ok_load_permanently = SQLITE_OK_LOAD_PERMANENTLY,
  error_missing_collseq = SQLITE_ERROR_MISSING_COLLSEQ,
  error_retry = SQLITE_ERROR_RETRY,
  error_snapshot = SQLITE_ERROR_SNAPSHOT,
  abort_rollback = SQLITE_ABORT_ROLLBACK,
  busy_recovery = SQLITE_BUSY_RECOVERY,
  busy_snapshot = SQLITE_BUSY_SNAPSHOT,
  locked_sharedcache = SQLITE_LOCKED_SHAREDCACHE,
  locked_vtab = SQLITE_LOCKED_VTAB,
  readonly_recovery = SQLITE_READONLY_RECOVERY,
  readonly_cantlock = SQLITE_READONLY_CANTLOCK,
  readonly_rollback = SQLITE_READONLY_ROLLBACK,
  readonly_dbmoved = SQLITE_READONLY_DBMOVED,
  readonly_cantinit = SQLITE_READONLY_CANTINIT,
  readonly_directory = SQLITE_READONLY_DIRECTORY,
  ioerr_read = SQLITE_IOERR_READ,
  ioerr_short_read = SQLITE_IOERR_SHORT_READ,
  ioerr_write = SQLITE_IOERR_WRITE,
  ioerr_fsync = SQLITE_IOERR_FSYNC,
  ioerr_dir_fsync = SQLITE_IOERR_DIR_FSYNC,
  ioerr_truncate = SQLITE_IOERR_TRUNCATE,
  ioerr_fstat = SQLITE_IOERR_FSTAT,
  ioerr_unlock = SQLITE_IOERR_UNLOCK,
  ioerr_rdlock = SQLITE_IOERR_RDLOCK,
  ioerr_delete = SQLITE_IOERR_DELETE,
  ioerr_blocked = SQLITE_IOERR_BLOCKED,
  ioerr_nomem = SQLITE_IOERR_NOMEM,
  ioerr_access = SQLITE_IOERR_ACCESS,
  ioerr_checkreservedlock = SQLITE_IOERR_CHECKRESERVEDLOCK,
  ioerr_lock = SQLITE_IOERR_LOCK,
  ioerr_close = SQLITE_IOERR_CLOSE,
  ioerr_dir_close = SQLITE_IOERR_DIR_CLOSE,
  ioerr_shmopen = SQLITE_IOERR_SHMOPEN,
  ioerr_shmsize = SQLITE_IOERR_SHMSIZE,
  ioerr_shmlock = SQLITE_IOERR_SHMLOCK,
  ioerr_shmmap = SQLITE_IOERR_SHMMAP,
  ioerr_seek = SQLITE_IOERR_SEEK,
  ioerr_delete_noent = SQLITE_IOERR_DELETE_NOENT,
  ioerr_mmap = SQLITE_IOERR_MMAP,
  ioerr_gettemppath = SQLITE_IOERR_GETTEMPPATH,
  ioerr_convpath = SQLITE_IOERR_CONVPATH,
  corrupt_vtab = SQLITE_CORRUPT_VTAB,
  corrupt_sequence = SQLITE_CORRUPT_SEQUENCE,
  cantopen_convpath = SQLITE_CANTOPEN_CONVPATH,
  cantopen_dirtywal = SQLITE_CANTOPEN_DIRTYWAL,
  cantopen_fullpath = SQLITE_CANTOPEN_FULLPATH,
  cantopen_isdir = SQLITE_CANTOPEN_ISDIR,
  cantopen_notempdir = SQLITE_CANTOPEN_NOTEMPDIR,
  constraint_check = SQLITE_CONSTRAINT_CHECK,
  constraint_commithook = SQLITE_CONSTRAINT_COMMITHOOK,
  constraint_foreignkey = SQLITE_CONSTRAINT_FOREIGNKEY,
  constraint_function = SQLITE_CONSTRAINT_FUNCTION,
  constraint_trigger = SQLITE_CONSTRAINT_TRIGGER,
  constraint_notnull = SQLITE_CONSTRAINT_NOTNULL,
  constraint_primarykey = SQLITE_CONSTRAINT_PRIMARYKEY,
  constraint_unique = SQLITE_CONSTRAINT_UNIQUE,
  constraint_vtab = SQLITE_CONSTRAINT_VTAB,
  constraint_rowid = SQLITE_CONSTRAINT_ROWID,
  notice_recover_wal = SQLITE_NOTICE_RECOVER_WAL,
  notice_recover_rollback = SQLITE_NOTICE_RECOVER_ROLLBACK,
  warning_autoindex = SQLITE_WARNING_AUTOINDEX,
};

constexpr result operator|(result l, result r) noexcept;
constexpr result operator&(result l, result r) noexcept;
constexpr result operator^(result l, result r) noexcept;
constexpr result operator~(result code) noexcept;

constexpr result operator|=(result& l, result r) noexcept;
constexpr result operator&=(result& l, result r) noexcept;
constexpr result operator^=(result& l, result r) noexcept;

inline zstring_view errstr(result code) noexcept;

constexpr result primary_result(result code) noexcept;

constexpr bool is_error(result code) noexcept;
constexpr bool is_non_error(result code) noexcept;


enum class open : int {
  readonly = SQLITE_OPEN_READONLY,
  readwrite = SQLITE_OPEN_READWRITE,
  create = SQLITE_OPEN_CREATE,
  deleteonclose = SQLITE_OPEN_DELETEONCLOSE,
  exclusive = SQLITE_OPEN_EXCLUSIVE,
  autoproxy = SQLITE_OPEN_AUTOPROXY,
  uri = SQLITE_OPEN_URI,
  memory = SQLITE_OPEN_MEMORY,
  main_db = SQLITE_OPEN_MAIN_DB,
  temp_db = SQLITE_OPEN_TEMP_DB,
  transient_db = SQLITE_OPEN_TRANSIENT_DB,
  main_journal = SQLITE_OPEN_MAIN_JOURNAL,
  temp_journal = SQLITE_OPEN_TEMP_JOURNAL,
  subjournal = SQLITE_OPEN_SUBJOURNAL,
  master_journal = SQLITE_OPEN_MASTER_JOURNAL,
  nomutex = SQLITE_OPEN_NOMUTEX,
  fullmutex = SQLITE_OPEN_FULLMUTEX,
  sharedcache = SQLITE_OPEN_SHAREDCACHE,
  privatecache = SQLITE_OPEN_PRIVATECACHE,
  wal = SQLITE_OPEN_WAL,
  nofollow = SQLITE_OPEN_NOFOLLOW,
};

constexpr open operator|(open l, open r) noexcept;
constexpr open operator&(open l, open r) noexcept;
constexpr open operator^(open l, open r) noexcept;
constexpr open operator~(open o) noexcept;

constexpr open operator|=(open& l, open r) noexcept;
constexpr open operator&=(open& l, open r) noexcept;
constexpr open operator^=(open& l, open r) noexcept;


enum class datatype : int {
  integer = SQLITE_INTEGER,
  _float = SQLITE_FLOAT,
  text = SQLITE3_TEXT,
  blob = SQLITE_BLOB,
  null = SQLITE_NULL
};


// =============================================================================


template <typename Enum, typename>
constexpr std::underlying_type_t<Enum> enum_to_int(Enum const value) noexcept
{
  return static_cast<std::underlying_type_t<Enum>>(value);
}

template <typename Enum, typename>
constexpr Enum int_to_enum(std::underlying_type_t<Enum> const value) noexcept
{
  return static_cast<Enum>(value);
}

constexpr result operator|(result const l, result const r) noexcept
{
  return int_to_enum<result>(enum_to_int(l) | enum_to_int(r));
}

constexpr result operator&(result const l, result const r) noexcept
{
  return int_to_enum<result>(enum_to_int(l) & enum_to_int(r));
}

constexpr result operator^(result const l, result const r) noexcept
{
  return int_to_enum<result>(enum_to_int(l) ^ enum_to_int(r));
}

constexpr result operator~(result const code) noexcept
{
  return int_to_enum<result>(~enum_to_int(code));
}

constexpr result operator|=(result& l, result const r) noexcept
{
  return l = l | r;
}

constexpr result operator&=(result& l, result const r) noexcept
{
  return l = l & r;
}

constexpr result operator^=(result& l, result const r) noexcept
{
  return l = l ^ r;
}

inline zstring_view errstr(result const code) noexcept
{
  return ::sqlite3_errstr(enum_to_int(code));
}

constexpr result primary_result(result const code) noexcept
{
  return code & result{ 0xFF };
}

constexpr bool is_error(result const code) noexcept
{
  auto const primary = primary_result(code);

  return !(primary == result::ok || primary == result::row
           || primary == result::done);
}

constexpr bool is_non_error(result const code) noexcept
{
  return !is_error(code);
}

constexpr open operator|(open const l, open const r) noexcept
{
  return int_to_enum<open>(enum_to_int(l) | enum_to_int(r));
}

constexpr open operator&(open const l, open const r) noexcept
{
  return int_to_enum<open>(enum_to_int(l) & enum_to_int(r));
}

constexpr open operator^(open const l, open const r) noexcept
{
  return int_to_enum<open>(enum_to_int(l) ^ enum_to_int(r));
}

constexpr open operator~(open const o) noexcept
{
  return int_to_enum<open>(~enum_to_int(o));
}

constexpr open operator|=(open& l, open const r) noexcept
{
  return l = l | r;
}

constexpr open operator&=(open& l, open const r) noexcept
{
  return l = l & r;
}

constexpr open operator^=(open& l, open const r) noexcept
{
  return l = l ^ r;
}


} // namespace sqleger


#endif
