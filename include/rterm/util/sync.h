#pragma once

#include <rterm/util/spin_lock.h>

#include <mutex>
#include <optional>
#include <shared_mutex>
#include <type_traits>

namespace rterm {

template<typename T, typename Mutex>
struct SyncResult {
    T value;
    std::unique_lock<Mutex> lock;
};

template<typename T, typename Mutex>
using OptionalSyncResult = std::optional<SyncResult<T, Mutex>>;

/* Inspired by Mutex<T> from Rust
 * Usage:
 *
 * Sync<YourType> sync;
 * ...
 *
 * {
 *     auto&& [value, lock] = sync.lock();
 *     ...
 *     // lock gets destructed at the end of the scope
 * }
 */
template<typename T, typename Mutex = std::mutex>
class Sync {
public:
    using value_type = T;
    using mutex_type = Mutex;

    using ConstRef = const T&;
    using MutRef = T&;
    using Result = SyncResult<ConstRef, Mutex>;
    using MutResult = SyncResult<MutRef, Mutex>;
    using OptionalResult = OptionalSyncResult<ConstRef, Mutex>;
    using OptionalMutResult = OptionalSyncResult<MutRef, Mutex>;

public:
    template<typename... Args>
    constexpr explicit Sync(Args&&... args)
        : mtx_{}
        , value_{std::forward<Args>(args)...} {
    }

    template<typename U, typename OtherMutex>
    Sync(const Sync<U, OtherMutex>& rhs) {
        operator=(rhs);
    }

    template<typename U, typename OtherMutex>
    Sync& operator=(const Sync<U, OtherMutex>& rhs) {
        auto&& [value, lock] = rhs.lock();
        value_ = value;
        return *this;
    }

    template<typename U, typename OtherMutex>
    Sync(Sync<U, OtherMutex>&& rhs) {
        operator=(std::move(rhs));
    }

    template<typename U, typename OtherMutex>
    Sync& operator=(Sync<U, OtherMutex>&& rhs) {
        auto&& [value, lock] = rhs.lock();
        value_ = std::move(value);
        return *this;
    }

    ~Sync() = default;


    Result lock() const {
        return {value_, std::unique_lock{mtx_}};
    }

    MutResult lock() {
        return {value_, std::unique_lock{mtx_}};
    }

    OptionalResult try_lock() const {
        if (mtx_.try_lock()) {
            return {value_, {mtx_, std::adopt_lock}};
        } else {
            return std::nullopt;
        }
    }

    OptionalMutResult try_lock() {
        if (mtx_.try_lock()) {
            return {value_, {mtx_, std::adopt_lock}};
        } else {
            return std::nullopt;
        }
    }

private:
    mutable Mutex mtx_;
    T value_;
};

enum class SharedState {
    shared,
    exclusive,
};

template<typename T, SharedState State, typename Mutex>
struct SharedSyncResult;
template<typename T, SharedState State, typename Mutex>
using OptionalSharedSyncResult = std::optional<SharedSyncResult<T, State, Mutex>>;

template<typename T, typename Mutex>
struct SharedSyncResult<T, SharedState::shared, Mutex> {
    T value;
    std::shared_lock<Mutex> lock;
};

template<typename T, typename Mutex>
struct SharedSyncResult<T, SharedState::exclusive, Mutex> {
    T value;
    std::unique_lock<Mutex> lock; /// scoped_lock is not moveable
};

template<typename T, typename Mutex = std::shared_mutex>
class SharedSync {
public:
    using value_type = T;
    using mutex_type = Mutex;

    using ConstRef = const T&;
    using MutRef = T&;

    template<typename U, SharedState State>
    using Result = SharedSyncResult<U, State, Mutex>;
    using SharedResult = Result<ConstRef, SharedState::shared>;
    using ExclusiveResult = Result<ConstRef, SharedState::exclusive>;
    using ExclusiveMutResult = Result<MutRef, SharedState::exclusive>;

    template<typename U, SharedState State>
    using OptionalResult = OptionalSharedSyncResult<U, State, Mutex>;
    using OptionalSharedResult = OptionalResult<ConstRef, SharedState::shared>;
    using OptionalExclusiveResult = OptionalResult<ConstRef, SharedState::exclusive>;
    using OptionalExclusiveMutResult = OptionalResult<MutRef, SharedState::exclusive>;

public:
    template<typename... Args>
    constexpr explicit SharedSync(Args&&... args)
        : mtx_{}
        , value_{std::forward<Args>(args)...} {
    }

    template<typename U, typename OtherMutex>
    SharedSync(const SharedSync<U, OtherMutex>& rhs) {
        operator=(rhs);
    }

    template<typename U, typename OtherMutex>
    SharedSync& operator=(const SharedSync<U, OtherMutex>& rhs) {
        auto&& [value, lock] = rhs.lock();
        value_ = value;
        return *this;
    }

    template<typename U, typename OtherMutex>
    SharedSync(SharedSync<U, OtherMutex>&& rhs) {
        operator=(std::move(rhs));
    }

    template<typename U, typename OtherMutex>
    SharedSync& operator=(SharedSync<U, OtherMutex>&& rhs) {
        auto&& [value, lock] = rhs.lock();
        value_ = std::move(value);
        return *this;
    }

    ~SharedSync() = default;

    /* Exclusive state */

    ExclusiveResult lock() const {
        return {value_, std::unique_lock{mtx_}};
    }

    ExclusiveMutResult lock() {
        return {value_, std::unique_lock{mtx_}};
    }

    OptionalExclusiveResult try_lock() const {
        if (mtx_.try_lock()) {
            return {value_, {mtx_, std::adopt_lock}};
        } else {
            return std::nullopt;
        }
    }

    OptionalExclusiveMutResult try_lock() {
        if (mtx_.try_lock()) {
            return {value_, {mtx_, std::adopt_lock}};
        } else {
            return std::nullopt;
        }
    }

    /* Shared state */

    SharedResult shared_lock() const {
        return {value_, std::shared_lock{mtx_}};
    }

    OptionalSharedResult shared_try_lock() const {
        if (mtx_.try_lock()) {
            return {value_, {mtx_, std::adopt_lock}};
        } else {
            return std::nullopt;
        }
    }

private:
    mutable Mutex mtx_{};
    T value_{};
};

} // namespace rterm
