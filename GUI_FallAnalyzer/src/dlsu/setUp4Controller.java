package dlsu;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Cursor;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;

import java.io.*;
import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by aenon on 11/11/2017.
 */
public class setUp4Controller implements Initializable {
    public Button first;
    public Button second;
    public Button third;
    public Button fourth;
    public Button finish;
    @FXML
    private TextField username;
    @FXML
    private PasswordField password;
    @FXML
    private Label feedbackLabel;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
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

        finish.setOnMouseEntered(event -> finish.setCursor(Cursor.HAND));
        finish.setOnMousePressed(event -> finish.setCursor(Cursor.CLOSED_HAND));
        finish.setOnMouseReleased(event -> finish.setCursor(Cursor.HAND));
        finish.setOnMouseExited(event -> finish.setCursor(Cursor.DEFAULT));
    }

    public void onFirst(ActionEvent actionEvent) throws IOException {
        String user = "";
        String pass = "";

        File loginFile = new File(selectCardController.driveLetter + "login.txt");
        if (loginFile.exists()){
            BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));
                user = br.readLine();
                pass = br.readLine();
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

            feedbackLabel.setText("");
            changeScene changeScene = new changeScene();
            changeScene.setScene("Setup1.fxml", "style.css", actionEvent, "Fall Analyzer | Login Credentials");

        }
    }

    public void onSecond(ActionEvent actionEvent) throws IOException {
        String user = "";
        String pass = "";

        File loginFile = new File(selectCardController.driveLetter + "login.txt");
        if (loginFile.exists()){
            BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));
                user = br.readLine();
                pass = br.readLine();
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

            feedbackLabel.setText("");
            changeScene changeScene = new changeScene();
            changeScene.setScene("Setup2.fxml", "style.css", actionEvent, "Fall Analyzer | Profile Setup");

        }
    }

    public void onThird(ActionEvent actionEvent) throws IOException {
        String user = "";
        String pass = "";

        File loginFile = new File(selectCardController.driveLetter + "login.txt");
        if (loginFile.exists()){
            BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));
                user = br.readLine();
                pass = br.readLine();
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

            feedbackLabel.setText("");
            changeScene changeScene = new changeScene();
            changeScene.setScene("Setup3.fxml", "style.css", actionEvent, "Fall Analyzer | Responders Setup");

        }
    }

    public void onFourth(ActionEvent actionEvent) throws IOException {
    }

    public void onFinish(ActionEvent actionEvent) throws IOException {
        String user = "";
        String pass = "";

        File loginFile = new File(selectCardController.driveLetter + "login.txt");
        if (loginFile.exists()){
            BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));
                user = br.readLine();
                pass = br.readLine();
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

            if (username.getText().isEmpty() || password.getText().isEmpty()){
                feedbackLabel.setText("All fields are required!");
            }else if(!username.getText().equals(user)){
                feedbackLabel.setText("Invalid username!");
            }else if(!password.getText().equals(pass)){
                feedbackLabel.setText("Invalid password!");
            }else{
                feedbackLabel.setText("");
                changeScene changeScene = new changeScene();
                changeScene.setScene("SetupFinish.fxml", "style.css", actionEvent, "Fall Analyzer | Remove card");
            }
        }
    }
}
