//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 1:49:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_RANGE_H__
#define __NKENTSEU_RANGE_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include "Utils.h"

namespace nkentseu {
    template<typename T>
    class NKENTSEU_API Range_ {
    private:
        T m_Min;
        T m_Max;

    public:
        // Default constructor
        Range_() : m_Min(0), m_Max(0) {}

        // Constructor with min and max values
        Range_(T min, T max) {
            if (min <= max) {
                m_Min = min;
                m_Max = max;
            }
            else {
                m_Min = max;
                m_Max = min;
            }
        }

        // Copy constructor
        Range_(const Range_<T>& other) : m_Min(other.m_Min), m_Max(other.m_Max) {}

        // Getter and setter for m_Min
        T GetMin() const {
            return m_Min;
        }

        void SetMin(T min) {
            m_Min = min;
            if (m_Min > m_Max) {
                T tmp = m_Max;
                m_Max = m_Min;
                m_Min = tmp;
            }
        }

        // Getter and setter for m_Max
        T GetMax() const {
            return m_Max;
        }

        void SetMax(T max) {
            m_Max = max;
            if (m_Max < m_Min) {
                T tmp = max;
                max = m_Min;
                m_Min = tmp;
            }
        }

        // Check if the range contains a value
        bool Contains(T value) const {
            return value >= m_Min && value <= m_Max;
        }

        // Check if this range contains another range
        bool Contains(const Range_<T>& other) const {
            return m_Min <= other.GetMin() && m_Max >= other.GetMax();
        }

        // Check if the range is null (min == max)
        bool IsNull() const {
            return m_Min == m_Max;
        }

        // Check if this range overlaps with another range
        bool Overlaps(const Range_<T>& other) const {
            return m_Min <= other.GetMax() && m_Max >= other.GetMin();
        }

        // Clamp a value to the range
        T Clamp(T value) const {
            if (value < m_Min) return m_Min;
            if (value > m_Max) return m_Max;
            return value;
        }

        // Get the length of the range
        T Len() const {
            return m_Max - m_Min;
        }

        // Find the intersection between two ranges
        static Range_<T> Intersect(const Range_<T>& range1, const Range_<T>& range2) {
            T min = maths::Max(range1.Min(), range2.Min());
            T max = maths::Min(range1.Max(), range2.Max());
            return Range_<T>(min, max);
        }

        // Find the union of two ranges
        static Range_<T> Union(const Range_<T>& range1, const Range_<T>& range2) {
            T min = maths::Min(range1.Min(), range2.Min());
            T max = maths::Max(range1.Max(), range2.Max());
            return Range_<T>(min, max);
        }

        // Check if two ranges form a single continuous range
        static bool UnionIsOneRange(const Range_<T>& range1, const Range_<T>& range2) {
            Range_<T> intersect = Intersect(range1, range2);
            return !intersect.IsNull();
        }

        static Range_<T> Hull(const Range_<T>& range1, const Range_<T>& range2) {
            T min = maths::Min(range1.GetMin(), range2.GetMin());
            T max = maths::Max(range1.GetMax(), range2.GetMax());
            return Range_<T>(min, max);
        }

        // Shift the range by a certain amount
        void Shift(T amount) {
            m_Min += amount;
            m_Max += amount;
        }

        // Split the range into smaller ranges based on a specified value
        // Returns a vector of Range objects
        static std::vector<Range_<T>> Split(const Range_<T>& range, T splitValue) {
            std::vector<Range_<T>> result;
            if (range.Contains(splitValue)) {
                result.push_back(Range_<T>(range.Min(), splitValue));
                result.push_back(Range_<T>(splitValue, range.Max()));
            }
            else {
                // Split value is outside the range, return the original range
                result.push_back(range);
            }
            return result;
        }

        // Expand the range by adding a margin around its limits
        void Expand(T margin) {
            m_Min -= margin;
            m_Max += margin;
        }

        // Get the center of the range
        T Center() const {
            return (m_Min + m_Max) / static_cast<T>(2);
        }
    };

    using RangeInt = Range_<int32>;
    using Range = Range_<float32>;
    using RangeFloat = Range_<float32>;
    using RangeDouble = Range_<float64>;
    using RangeUInt = Range_<uint32>;
} // namespace nkentseu

#endif    // __NKENTSEU_RANGE_H__