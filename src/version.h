/*
 * SST MTS Source - an editable morphable tuning source
 * for the ODDSound MTS-ESP protocol.
 *
 * Learn more at https://surge-synth-team.org/
 *
 * Copyright 2023, various authors, as described in the GitHub
 * transaction log.
 *
 * SST MTS Source is released under the GNU General Public Licence v3
 * or later (GPL-3.0-or-later). The license is found in the "LICENSE"
 * file in the root of this repository, or at
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * All source for SST MTS Source is available at
 * https://github.com/surge-synthesizer/sst-mts-source
 */
#ifndef SSTMTSSOURCE_SRC_VERSION_H
#define SSTMTSSOURCE_SRC_VERSION_H
#include <string>

namespace Build
{
extern const std::string build_date;
extern const std::string git_branch;
extern const std::string git_commit_hash;
} // namespace Build

#endif // SSTMTSSOURCE_SRC_VERSION_H
