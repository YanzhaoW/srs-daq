import './App.css'
import Headline from './components/Headline'
import Sidebar from './components/SideBar'
import MainRegion from "./components/MainRegion"
import { useState } from "react";
import { ActiveTab } from "./utils/CommonDefinitions"


function App() {
    const [activeTab, setActiveTab] = useState<ActiveTab>(ActiveTab.visualization);

    return (
        <>
            <Headline setter={setActiveTab} />
            <div>
                <Sidebar />
                <MainRegion activeTab = {activeTab}/>
            </div>
        </>
    )
}

export default App
