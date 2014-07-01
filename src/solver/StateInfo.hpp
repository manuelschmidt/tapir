/** file: StateInfo.hpp
 *
 * Contains the StateInfo class, which is a wrapper for an individual state of the PODMP.
 *
 * The StateInfo also keeps track of which history entries involve that state, which allows for
 * an efficient way to look up which history entries are affected by certain changes in the model.
 */
#ifndef SOLVER_STATEINFO_HPP_
#define SOLVER_STATEINFO_HPP_

#include <memory>                       // for unique_ptr
#include <unordered_set>                          // for seteset(state.copy());
#include <vector>                       // for vector

#include "global.hpp"

#include "solver/changes/ChangeFlags.hpp"               // for ChangeFlags
#include "solver/abstract-problem/State.hpp"

namespace solver {
class BeliefNode;
class HistoryEntry;

/** A wrapper for a POMDP state.
 *
 * Each StateInfo should be stored inside a state pool; its ID will be its position in the
 * pool's vector of states, which allows efficient lookup of state by ID.
 */
class StateInfo {
public:
    friend class HistoryEntry;
    friend class Simulator;
    friend class Solver;
    friend class StatePool;
    friend class TextSerializer;

    /** Constructs a StateInfo with no associated state!! */
    StateInfo();
    /** Constructs a StateInfo to manage the given state. */
    StateInfo(std::unique_ptr<State> state);
    /** Constructs a StateInfo to manage the given state. */
    StateInfo(State const &state);

    /** Default destructor. */
    ~StateInfo();
    _NO_COPY_OR_MOVE(StateInfo);

    /* ---------------------- Simple getters  ---------------------- */
    /** Returns the ID of this state. */
    long getId() const;
    /** Returns the state held by this StateInfo. */
    State const *getState() const;

private:
    /* ----------------- History entry registration  ----------------- */
    /** Registers a history entry as containing this state. */
    void addHistoryEntry(HistoryEntry *entry);
    /** Deregisters a history entry that no longer contains this state. */
    void removeHistoryEntry(HistoryEntry *entry);

    /* ---------------------- Model change handling  ---------------------- */
    /** Resets the change flags for this state. */
    void resetChangeFlags();
    /** Sets the given flags for this state. */
    void setChangeFlags(ChangeFlags flags);

private:
    /** The underlying state wrapped by this StateInfo. */
    std::unique_ptr<State const> state_;
    /** The ID of this StateInfo, which is also its index in the state pool. */
    long id_;

    /** The set of history entries that this state occurs in. */
    std::unordered_set<HistoryEntry *> usedInHistoryEntries_;

    /** The flags for the changes that affect this state. */
    ChangeFlags changeFlags_;
};
} /* namespace solver */

#endif /* SOLVER_STATEINFO_HPP_ */
