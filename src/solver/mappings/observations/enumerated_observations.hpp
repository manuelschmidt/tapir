/** file: enumerated_observations.hpp
 *
 * Provides an implementation of the observation mapping interface that is designed for an
 * enumerated set of observations - this set should generally be relatively small, and they should
 * occur relatively non-sparsely.
 *
 * Using this implementation requires that the observations implement DiscretizedPoint, which
 * allows the mapping to determine the enumeration number associated with any observation. Each
 * mapping simply stores its entries in an array, where the array index is the enumeration number.
 */
#ifndef SOLVER_ENUMERATED_OBSERVATIONS_HPP_
#define SOLVER_ENUMERATED_OBSERVATIONS_HPP_

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "solver/BeliefNode.hpp"

#include "solver/abstract-problem/Model.hpp"

#include "solver/serialization/Serializer.hpp"

#include "solver/mappings/observations/ObservationPool.hpp"
#include "solver/mappings/observations/ObservationMapping.hpp"

namespace solver {
class ActionPool;
class DiscretizedPoint;
class EnumeratedObservationMapEntry;

/** An implementation of the ObservationPool interface that is based on an enumerated set of
 * observations.
 *
 * All that is needed for this is a vector containing all of the individual observations, which
 * should be in the order of enumeration.
 */
class EnumeratedObservationPool: public solver::ObservationPool {
  public:
    /** Creates a new EnumeratedObservationPool with the given set of observations, in the given
     * order.
     */
    EnumeratedObservationPool(std::vector<std::unique_ptr<DiscretizedPoint>> observations);
    virtual ~EnumeratedObservationPool() = default;
    _NO_COPY_OR_MOVE(EnumeratedObservationPool);

    /** Creates a new observation mapping for the given ActionNode. */
    virtual std::unique_ptr<ObservationMapping> createObservationMapping(ActionNode *owner) override;

  private:
    /** All of the observations in their enumerated order. */
    std::vector<std::unique_ptr<DiscretizedPoint>> observations_;
};

/** A concrete class implementing ObservationMapping for an enumerated set of observations.
 *
 * This class stores its mapping entries in an array, whose size is determined at runtime based on
 * the number of entries in the observation vector of the EnumeratedObservationPool.
 */
class EnumeratedObservationMap: public solver::ObservationMapping {
  public:
    friend class EnumeratedObservationMapEntry;
    friend class EnumeratedObservationTextSerializer;
    EnumeratedObservationMap(ActionNode *owner,
            std::vector<std::unique_ptr<DiscretizedPoint>> const &allObservations);

    // Default destructor; copying and moving disallowed!
    virtual ~EnumeratedObservationMap() = default;
    _NO_COPY_OR_MOVE(EnumeratedObservationMap);

    /* -------------- Creation and retrieval of nodes. ---------------- */
    virtual BeliefNode *getBelief(Observation const &obs) const override;
    virtual BeliefNode *createBelief(Observation const &obs) override;
    virtual long getNChildren() const override;

    /* -------------- Retrieval of mapping entries. ---------------- */
    virtual ObservationMappingEntry *getEntry(Observation const &obs) override;
    virtual ObservationMappingEntry const *getEntry(Observation const &obs) const override;
    virtual std::vector<ObservationMappingEntry const *> getAllEntries() const override;

    /* ------------- Methods for accessing visit counts. --------------- */
    virtual long getTotalVisitCount() const override;

  private:
    /** A reference back to the vector of observations in the pool. */
    std::vector<std::unique_ptr<DiscretizedPoint>> const &allObservations_;

    /** The number of observations in this mapping. */
    long nObservations_;
    /** An array of all of the child entries in this mapping. */
    std::unique_ptr<EnumeratedObservationMapEntry[]> entries_;

    /** The number of child nodes for this mapping - not all entries will have a child node! */
    long nChildren_;
    /** The total visit count over all observations in this mapping. */
    long totalVisitCount_;
};


/** A concrete class implementing ObservationMappingEntry for an enumerated set of observations.
 *
 * Each entry stores its enumeration number and a pointer back to its parent map,
 * as well as a child node and visit count.
 */
class EnumeratedObservationMapEntry : public solver::ObservationMappingEntry {
    friend class EnumeratedObservationMap;
    friend class EnumeratedObservationTextSerializer;

public:
    virtual ObservationMapping *getMapping() const override;
    virtual std::unique_ptr<Observation> getObservation() const override;
    virtual BeliefNode *getBeliefNode() const override;
    virtual long getVisitCount() const override;

    virtual void updateVisitCount(long deltaNVisits) override;

private:
    /** The parent mapping for this entry. */
    EnumeratedObservationMap *map_ = nullptr;
    /** The enumeration number for this entry - this should be its index into the vector of
     * observations and also its index in the array of entries in the parent mapping.
     */
    long index_ = 0;
    /** The child node for this entry, if it has one. */
    std::unique_ptr<BeliefNode> childNode_ = nullptr;
    /** The visit count for this entry. */
    long visitCount_ = 0;
};

/** A partial implementation of the Serializer interface which provides serialization methods for
 * the above enumerated observation mapping classes.
 */
class EnumeratedObservationTextSerializer: virtual public solver::Serializer {
  public:
    EnumeratedObservationTextSerializer() = default;
    virtual ~EnumeratedObservationTextSerializer() = default;
    _NO_COPY_OR_MOVE(EnumeratedObservationTextSerializer);

    virtual void saveObservationPool(
            ObservationPool const &observationPool, std::ostream &os) override;
    virtual std::unique_ptr<ObservationPool> loadObservationPool(
            std::istream &is) override;
    virtual void saveObservationMapping(ObservationMapping const &map,
            std::ostream &os) override;
    virtual std::unique_ptr<ObservationMapping> loadObservationMapping(ActionNode *owner,
            std::istream &is) override;
};
} /* namespace solver */

#endif /* SOLVER_ENUMERATED_OBSERVATIONS_HPP_ */
