#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <algorithm>

#include "Range.h"

using std::string;
using std::vector;

class CallEntry
{
public:
    CallEntry(const string &functionName, const Range &functionRange) : name(functionName),
                                                        range(functionRange)
    {
        timestamp = time(nullptr);
    }

    const string &getName() const { return name; }
    const Range &getRange() const { return range; }
    time_t getTimestamp() const { return timestamp; }

private:
    string name;
    Range range;
    time_t timestamp;
};

class CallStack
{
public:
    void push(const CallEntry &entry) { entries.push_back(entry); }
    void push(const string &name, const Range &range)
    {
        entries.emplace_back(name, range);
    }
    void pop() { entries.pop_back(); }
    const CallEntry &top() const { return entries.back(); }

    void clear() { entries.clear(); }
    bool isEmpty() const { return entries.empty(); }
    size_t size() const { return entries.size(); }

    string toString(const string &format = "table") const
    {
        if (entries.empty())
        {
            return "Call Stack is empty\n";
        }

        if (format == "raw")
        {
            string result = "Call Stack:\n";
            for (const auto &entry : entries)
            {
                result += "    at " + entry.getName() + " (" + entry.getRange().getStart().toString() + ")\n";
            }
            return result;
        }

        // Default table format
        string result;

        // Find the maximum width for function names and ranges for proper alignment
        size_t maxNameWidth = 8;  // Minimum width for "Function" header
        size_t maxRangeWidth = 8; // Minimum width for "Location" header

        for (const auto &entry : entries)
        {
            maxNameWidth = std::max(maxNameWidth, entry.getName().length());
            maxRangeWidth = std::max(maxRangeWidth, entry.getRange().getStart().toString().length());
        }

        // Calculate total table width: 2 spaces padding per column + 3 separators + 2 borders
        size_t tableWidth = maxNameWidth + maxRangeWidth + 7;

        // Create top border
        result += "┌";
        for (size_t i = 0; i < tableWidth - 2; ++i)
        {
            result += "─";
        }
        result += "┐\n";

        // Center "Call Stack" in the header
        string header = "Call Stack";
        size_t headerPadding = (tableWidth - 2 - header.length()) / 2;
        result += "│";
        for (size_t i = 0; i < headerPadding; ++i)
        {
            result += " ";
        }
        result += header;
        for (size_t i = headerPadding + header.length(); i < tableWidth - 2; ++i)
        {
            result += " ";
        }
        result += "│\n";

        // Separator between header and column titles
        result += "├";
        for (size_t i = 0; i < maxNameWidth + 2; ++i) // +2 for padding
        {
            result += "─";
        }
        result += "┬";
        for (size_t i = 0; i < maxRangeWidth + 2; ++i) // +2 for padding
        {
            result += "─";
        }
        result += "┤\n";

        // Column headers
        result += "│ Function";
        for (size_t i = 8; i < maxNameWidth + 1; ++i) // +1 for right padding
        {
            result += " ";
        }
        result += "│ Location";
        for (size_t i = 8; i < maxRangeWidth + 1; ++i) // +1 for right padding
        {
            result += " ";
        }
        result += "│\n";

        // Separator between headers and data
        result += "├";
        for (size_t i = 0; i < maxNameWidth + 2; ++i) // +2 for padding
        {
            result += "─";
        }
        result += "┼";
        for (size_t i = 0; i < maxRangeWidth + 2; ++i) // +2 for padding
        {
            result += "─";
        }
        result += "┤\n";

        // Add entries (newest at bottom like a log file)
        for (const auto &entry : entries)
        {
            result += "│ " + entry.getName();
            for (size_t i = entry.getName().length(); i < maxNameWidth + 1; ++i) // +1 for right padding
            {
                result += " ";
            }
            result += "│ " + entry.getRange().getStart().toString();
            for (size_t i = entry.getRange().getStart().toString().length(); i < maxRangeWidth + 1; ++i) // +1 for right padding
            {
                result += " ";
            }
            result += "│\n";
        }

        // Bottom border
        result += "└";
        for (size_t i = 0; i < maxNameWidth + 2; ++i) // +2 for padding
        {
            result += "─";
        }
        result += "┴";
        for (size_t i = 0; i < maxRangeWidth + 2; ++i) // +2 for padding
        {
            result += "─";
        }
        result += "┘\n";

        return result;
    }

private:
    vector<CallEntry> entries;
};