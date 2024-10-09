#pragma once

#ifdef HAS_ROOT
#include "DataStructs.hpp"
#include <TFile.h>
#include <TTree.h>

namespace srs
{
    class RootFileWriter
    {
      public:
        explicit RootFileWriter(auto&&... args)
            : root_file{ std::forward<decltype(args)>(args)... }
        {
            tree.SetDirectory(&root_file);
        }

        RootFileWriter(const RootFileWriter&) = delete;
        RootFileWriter(RootFileWriter&&) = delete;
        RootFileWriter& operator=(const RootFileWriter&) = delete;
        RootFileWriter& operator=(RootFileWriter&&) = delete;
        ~RootFileWriter() { root_file.Write(); }

        void register_branch(ExportData& data) { tree.Branch("srs_frame_data", &data); }

        void fill() { tree.Fill(); }

      private:
        TFile root_file;
        TTree tree{ "srs_data_tree", "Data structures from SRS system" };
    };

} // namespace srs

#endif
