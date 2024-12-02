import Plots from './Plots.tsx'
import ControlTab from './Control'
import HelpTab from './Help'
import { ActiveTab } from "../utils/CommonDefinitions"

interface MainRegionProps {
    activeTab: ActiveTab;
};


const MainRegion = ({ activeTab }: MainRegionProps) => {

    switch (activeTab) {
        case ActiveTab.visualization:
            return <Plots />
        case ActiveTab.control:
            return <ControlTab />
        case ActiveTab.help:
            return <HelpTab />
    }

};

export default MainRegion
