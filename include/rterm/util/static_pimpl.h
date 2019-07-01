#pragma once

#include <cstddef>
#include <memory>
#include <utility>

namespace rterm {

template<typename Derived, size_t ImplSize = 64>
class StaticPimpl {
public:
    template<typename ...Args>
    StaticPimpl(Args&& ...args) {
        Derived::assertImplSize(ImplSize);
        derived().init(std::forward<Args>(args)...);
    }

    ~StaticPimpl() {
        derived().destroy();
    }

    StaticPimpl(StaticPimpl&&) = default;
    StaticPimpl& operator=(StaticPimpl&&) = default;
    StaticPimpl(const StaticPimpl&) = delete;
    StaticPimpl& operator=(const StaticPimpl&) = delete;

    static constexpr size_t Size = ImplSize;

protected:
    template<typename Impl>
    Impl& impl() {
        return const_cast<Impl&>(const_cast<const StaticPimpl*>(this)->impl());
    }

    template<typename Impl>
    const Impl& impl() const {
        return *std::launder<Impl*>(&storage_);
    }

    const auto* storage() const {
        return &storage_;
    }

    auto* storage() {
        return &storage_;
    }

private:
    Derived& derived() {
        return static_cast<Derived&>(*this);
    }

private:
    static constexpr size_t ImplAlign{alignof(std::max_align_t)};
    std::aligned_storage_t<ImplSize, ImplAlign> storage_;
};

} // namespace rterm
