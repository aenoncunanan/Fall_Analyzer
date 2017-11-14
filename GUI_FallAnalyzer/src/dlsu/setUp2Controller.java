package dlsu;

import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.Cursor;
import javafx.scene.control.*;

import java.io.*;
import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by aenon on 11/11/2017.
 */
public class setUp2Controller implements Initializable {
    public Button first;
    public Button second;
    public Button third;
    public Button fourth;
    public ChoiceBox gender;
    public TextField firstName;
    public TextField LastName;
    public TextField age;
    public TextField address;
    public TextField contactNumber;
    public Label feedbackLabel;
    public Button finish;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        File file = new File(selectCardController.driveLetter + "profile.txt");
        if(file.exists()){
            BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "profile.txt"));

                firstName.setText(br.readLine());
                LastName.setText(br.readLine());
                gender.setValue(br.readLine());
                age.setText(br.readLine());
                address.setText(br.readLine());
                contactNumber.setText(br.readLine());

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

        gender.getItems().add("male");
        gender.getItems().add("female");

        File respondentsFile = new File(selectCardController.driveLetter + "respondents.txt");
        if (!respondentsFile.exists()){
            fourth.setDisable(true);
        }

        finish.setDisable(true);

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
        changeScene changeScene = new changeScene();
        changeScene.setScene("Setup1.fxml", "style.css", actionEvent, "Fall Analyzer | Login Credentials");
    }

    public void onSecond(ActionEvent actionEvent) {
    }

    public void onThird(ActionEvent actionEvent) throws IOException {
        if(firstName.getText().isEmpty() || LastName.getText().isEmpty() || age.getText().isEmpty() || address.getText().isEmpty() || contactNumber.getText().isEmpty()){
            feedbackLabel.setText("All fields are required!");
        } else{
            try{
                Integer.parseInt(age.getText());

                BufferedWriter writer = null;
                try{
                    writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "profile.txt"), "utf-8"));
                    writer.write(firstName.getText());
                    writer.newLine();
                    writer.write(LastName.getText());
                    writer.newLine();
                    writer.write(gender.getValue().toString());
                    writer.newLine();
                    writer.write(age.getText());
                    writer.newLine();
                    writer.write(address.getText());
                    writer.newLine();
                    writer.write(contactNumber.getText());
                    writer.close();
                } catch(Exception e){
                }
            }catch(Exception e){
                feedbackLabel.setText("Your age is invalid!");
            }finally {
                feedbackLabel.setText("");
                changeScene changeScene = new changeScene();
                changeScene.setScene("Setup3.fxml", "style.css", actionEvent,"Fall Analyzer | Responders Setup");
            }
        }
    }

    public void onFourth(ActionEvent actionEvent) throws IOException {
        changeScene changeScene = new changeScene();
        changeScene.setScene("Setup4.fxml", "style.css", actionEvent, "Fall Analyzer | Finish Setup");
    }

    public void onFinish(ActionEvent actionEvent) {

    }
}
