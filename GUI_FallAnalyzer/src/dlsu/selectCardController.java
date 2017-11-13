package dlsu;


import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Cursor;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.stage.Stage;

import java.io.*;
import java.net.URL;
import java.util.*;

public class selectCardController implements Initializable{
    dlsu.getExternalDevices getExternalDevices = new getExternalDevices();

    @FXML
    private Button onNext;

    @FXML
    private Label feedbackLabel;

    @FXML
    private ComboBox selectCard;

    public static String driveLetter;


    @Override
    public void initialize(URL location, ResourceBundle resources) {
        getExternalDevices.getDevices();
        System.out.println("Storage Devices: " + getExternalDevices.storages);
        for (int i = 0; i < getExternalDevices.storages.size(); i++){
            selectCard.getItems().add(getExternalDevices.storages.get(i));
        }
        onNext.setOnMouseEntered(event -> onNext.setCursor(Cursor.HAND));
        onNext.setOnMousePressed(event -> onNext.setCursor(Cursor.CLOSED_HAND));
        onNext.setOnMouseReleased(event -> onNext.setCursor(Cursor.HAND));
        onNext.setOnMouseExited(event -> onNext.setCursor(Cursor.DEFAULT));

        selectCard.setOnMouseEntered(event -> selectCard.setCursor(Cursor.HAND));
        selectCard.setOnMousePressed(event -> selectCard.setCursor(Cursor.CLOSED_HAND));
        selectCard.setOnMouseReleased(event -> selectCard.setCursor(Cursor.HAND));
        selectCard.setOnMouseExited(event -> selectCard.setCursor(Cursor.DEFAULT));
    }

    @FXML
    private void onNext(ActionEvent actionEvent) throws IOException{
        List<String> messages = new ArrayList<String>();

        messages.add("Please select your card first!");
        messages.add("You haven't selected your card yet.");
        messages.add("Ooopsie! Please select your card please.");
        messages.add("Select your card first.");

        Collections.shuffle(messages);

        try{
            String drive = selectCard.getValue().toString();
            driveLetter = "";
            for (int i = 0; i < drive.length(); i++){
                if (Objects.equals(drive.charAt(i), '(')){
                    i++;
                    driveLetter = driveLetter + drive.charAt(i) + ":" + "\\";
                }
            }
            System.out.println("Selected Drive: " + drive);
            System.out.println("Selected Drive Letter: " + driveLetter);

            File loginFile = new File(driveLetter + "login.txt");
            if(loginFile.exists()){
                feedbackLabel.setText("");
                dlsu.changeScene changeScene = new changeScene();
                changeScene.setScene("Explore.fxml", "style.css", actionEvent, "Fall Analyzer");
            } else{
                feedbackLabel.setText("");
                dlsu.changeScene changeScene = new changeScene();
                changeScene.setScene("SetupHome.fxml", "style.css", actionEvent, "Fall Analyzer | Home Setup");
            }

        } catch (Exception e){
            feedbackLabel.setText(messages.get(0));
        }

    }

    public void onSelect(ActionEvent actionEvent) {
        List<String> messages = new ArrayList<String>();

        messages.add("Great! Click next to proceed");
        messages.add("Ok! Now, click next to proceed");
        messages.add("Alright! To proceed, click next");
        messages.add("Let's proceed by clicking the next button");

        Collections.shuffle(messages);

        feedbackLabel.setText(messages.get(0));
    }

}
