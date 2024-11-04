#pragma once

#ifdef HAS_ROOT
#include "DataStructs.hpp"
#include <TFile.h>
#include <TTree.h>
#include <srs/analysis/DataProcessManager.hpp>

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

        static constexpr auto IsStructType = true;
        RootFileWriter(const RootFileWriter&) = delete;
        RootFileWriter(RootFileWriter&&) = delete;
        RootFileWriter& operator=(const RootFileWriter&) = delete;
        RootFileWriter& operator=(RootFileWriter&&) = delete;
        ~RootFileWriter() { root_file.Write(); }
        auto write(auto fut) -> boost::unique_future<std::optional<int>> { return {}; }
        [[nodiscard]] static auto get_deserialize_mode()  -> DataDeserializeOptions { return DataDeserializeOptions::structure; }

        // The data register with Branch must be non-const, which is insane.
        // NOLINTBEGIN
        void register_branch(const StructData& data) { tree.Branch("srs_frame_data", const_cast<StructData*>(&data)); }
        // NOLINTEND

        void fill() { tree.Fill(); }

      private:
        TFile root_file;
        TTree tree{ "srs_data_tree", "Data structures from SRS system" };
    };

} // namespace srs

#endif
