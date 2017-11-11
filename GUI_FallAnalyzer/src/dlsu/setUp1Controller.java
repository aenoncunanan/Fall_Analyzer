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
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.*;
import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by aenon on 11/11/2017.
 */
public class setUp1Controller implements Initializable {
    public Button first;
    public Button second;
    public Button third;
    public Button fourth;
    @FXML
    private TextField username;
    @FXML
    private PasswordField confirm;
    @FXML
    private PasswordField password;
    @FXML
    private Label feedbackLabel;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        File loginFile = new File(selectCardController.driveLetter + "login.txt");
        if(loginFile.exists()){
            BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));

                username.setText(br.readLine());
                String temp = br.readLine();
                password.setText(temp);
                confirm.setText(temp);

            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        File profileFile = new File(selectCardController.driveLetter + "profile.txt");
        if (!profileFile.exists()){
            third.setDisable(true);
        }

        File respondentsFile = new File(selectCardController.driveLetter + "respondents.txt");
        if (!respondentsFile.exists()){
            fourth.setDisable(true);
        }

        first.setOnMouseEntered(event -> first.setCursor(Cursor.HAND));
        first.setOnMousePressed(event -> first.setCursor(Cursor.CLOSED_HAND));
        first.setOnMouseReleased(event -> first.setCursor(Cursor.HAND));
        first.setOnMouseExited(event -> first.setCursor(Cursor.DEFAULT));

        second.setOnMouseEntered(event -> second.setCursor(Cursor.HAND));
        second.setOnMousePressed(event -> second.setCursor(Cursor.CLOSED_HAND));
        second.setOnMouseReleased(event -> second.setCursor(Cursor.HAND));
        second.setOnMouseExited(event -> second.setCursor(Cursor.DEFAULT));

        third.setOnMouseEntered(event -> third.setCursor(Cursor.HAND));
        third.setOnMousePressed(event -> third.setCursor(Cursor.CLOSED_HAND));
        third.setOnMouseReleased(event -> third.setCursor(Cursor.HAND));
        third.setOnMouseExited(event -> third.setCursor(Cursor.DEFAULT));

        fourth.setOnMouseEntered(event -> fourth.setCursor(Cursor.HAND));
        fourth.setOnMousePressed(event -> fourth.setCursor(Cursor.CLOSED_HAND));
        fourth.setOnMouseReleased(event -> fourth.setCursor(Cursor.HAND));
        fourth.setOnMouseExited(event -> fourth.setCursor(Cursor.DEFAULT));
    }

    public void onFirst(ActionEvent actionEvent) {
    }

    public void onSecond(ActionEvent actionEvent) throws IOException {
        if (username.getText().isEmpty() || password.getText().isEmpty() || confirm.getText().isEmpty()){
            feedbackLabel.setText("All fields are required!");
        }else{
            if(password.getText().equals(confirm.getText())){
                feedbackLabel.setText("");
                BufferedWriter writer = null;

                try{
                    writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "login.txt"), "utf-8"));
                    writer.write(username.getText());
                    writer.newLine();
                    writer.write(password.getText());
                    writer.close();
                }catch(Exception e){
                }

                dlsu.changeScene changeScene = new changeScene();
                changeScene.setScene("Setup2.fxml", "style.css", actionEvent);

            }else {
                feedbackLabel.setText("Your passwords doesn't match!");
            }
        }

    }

    public void onThird(ActionEvent actionEvent) {
    }

    public void onFourth(ActionEvent actionEvent) {
    }
}
