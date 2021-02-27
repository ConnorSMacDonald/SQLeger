#ifndef SQLEGER_SENTINELS_HPP
#define SQLEGER_SENTINELS_HPP


namespace sqleger {


struct null_t {
};

static constexpr null_t null {};


struct skip_t {
  template <typename T>
  constexpr skip_t const& operator=(T const&) const noexcept
  {
    return *this;
  }
};

static constexpr skip_t skip {};


} // namespace sqleger


#endif
