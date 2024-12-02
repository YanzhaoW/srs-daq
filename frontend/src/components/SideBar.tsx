import styles from './SideBar.module.css'
import BillboardJS, { IChart } from "@billboard.js/react";
import bb, { gauge } from "billboard.js"
import { useRef, useEffect } from "react";
import "billboard.js/dist/billboard.css";

const SideBar = () => {

    const gaugeChartByte = useRef<IChart>(null);

    const gaugeChartHit = useRef<IChart>(null);

    const options = {
        size: {
            height: 120,
            width: 180,
        },
        data: {
            columns: [
                []
            ],
            type: gauge()
        },


    };

    useEffect(() => {
        const chart = gaugeChartByte.current?.instance;
        if (chart) {
            chart.load({ columns: [["Received bytes / sec", 250]] });
        }
    }, []);

    useEffect(() => {
        const chart = gaugeChartHit.current?.instance;
        if (chart) {
            chart.load({ columns: [["Hits / sec", 250]] });
        }
    }, []);

    return (
        <div className={styles.parent}>
            <div style={{ textAlign: "center" }}>
                <div className={styles.start_div}>
                    <button className={styles.start_button}>Start</button><br />
                    <p className={styles.start_button_caption}>Press to start</p>
                </div>
            </div>
            <div className={styles.gauge_chart}>
                <div style={{ display: "inline-block" }}>
                    <BillboardJS
                        bb={bb}
                        options={options}
                        ref={gaugeChartByte}
                        className={"bb"}
                    />
                </div>
                <div style={{ display: "inline-block" }}>
                    <BillboardJS
                        bb={bb}
                        options={options}
                        ref={gaugeChartHit}
                        className={"bb"}
                    />
                </div>
            </div>
        </div>
    );
}

export default SideBar;
