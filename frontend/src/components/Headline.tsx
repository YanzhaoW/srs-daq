import styles from './Headline.module.css'
import { Dispatch, SetStateAction } from "react";
import { ActiveTab } from "../utils/CommonDefinitions"

interface HeadlineProps {
    setter: Dispatch<SetStateAction<ActiveTab>>;
};

const Headline = ({ setter }: HeadlineProps) => {

    const SetTabActive = (tab: ActiveTab) => { setter(tab) };

    return (
        <div className={styles.headline}>
            <span className={styles.icon_text}>SRS Web Application</span>
            <div className={styles.log_window}>
                {
                    `
        log infomation ......
            log infomation ......
            log infomation ......
            log infomation ......
            log infomation ......
            log infomation ......
            `
                }
            </div>
            <div className={styles.right_div}>
                <div className={styles.tabs_div}>
                    <button className={styles.button} onClick={() => { SetTabActive(ActiveTab.control) }} >Control</button>
                    <button className={styles.button} onClick={() => { SetTabActive(ActiveTab.visualization) }}>Visualization</button>
                    <button className={styles.button} onClick={() => { SetTabActive(ActiveTab.help) }}>Help</button>
                </div>
            </div>
        </div>
    );
}

export default Headline
