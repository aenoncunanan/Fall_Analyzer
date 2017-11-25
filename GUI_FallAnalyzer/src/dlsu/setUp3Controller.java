package dlsu;

import dlsu.Utils.checkContactNumber;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
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
public class setUp3Controller implements Initializable {
    public Button first;
    public Button second;
    public Button third;
    public Button fourth;
    public TextField firstName;
    public TextField LastName;
    public TextField contactNumber;
    public Label feedbackLabel;
    public ListView list;

    public final ObservableList <responders> data = FXCollections.observableArrayList();
    public Button finish;

    public static class responders{
        private SimpleStringProperty firstName;
        private SimpleStringProperty lastName;
        private SimpleStringProperty contactNumber;

        private responders(String first, String last, String contact){
            this.firstName = new SimpleStringProperty(first);
            this.lastName = new SimpleStringProperty(last);
            this.contactNumber = new SimpleStringProperty(contact);
        }

        public String getFirstName(){
            return firstName.get();
        }

        public void setFirstName(String first){
            firstName.set(first);
        }

        public String getLastName(){
            return lastName.get();
        }

        public void setLastName(String last){
            lastName.set(last);
        }

        public String getContactNumber(){
            return contactNumber.get();
        }

        public void setContactNumber(String contact){
            contactNumber.set(contact);
        }
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        File respondentsFile = new File(selectCardController.driveLetter + "respondents.txt");
        if (respondentsFile.exists()){
            BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "respondents.txt"));

                Boolean end = false;
                String line;
                String first = "";
                String last = "";
                String contact = "";
                while(end == false){
                    line = br.readLine();
                    if (line.equals("-end-")){
                        end = true;
                    } else if (line.equals("breakLine")){
                        //just do nothing
                    } else{
                        first = line;
                        last = br.readLine();
                        contact = br.readLine();
                        data.add(new responders(first, last, contact));
                        list.getItems().add(first + " " + last);
                    }
                }
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

    public void onAdd(ActionEvent actionEvent) {
        if (firstName.getText().isEmpty() || LastName.getText().isEmpty() || contactNumber.getText().isEmpty()){
            feedbackLabel.setText("All fields are required!");
        }else{
            if (checkContactNumber.validContactNumber(contactNumber.getText())){
                list.getItems().add(firstName.getText() + " " + LastName.getText());
                data.add(new responders(firstName.getText(), LastName.getText(), contactNumber.getText()));
                firstName.setText("");
                LastName.setText("");
                contactNumber.setText("+639");
                feedbackLabel.setText("");
            }else {
                feedbackLabel.setText("Your contact number is invalid!");
                contactNumber.setText("+639");
            }
        }
    }

    public void onDelete(ActionEvent actionEvent) {
        try{
            int index   = list.getSelectionModel().getSelectedIndex();
            list.getItems().remove(index);
            data.remove(index);
        }catch (Exception e){
        }
    }

    public void onEdit(ActionEvent actionEvent) {
        try{
            int index   = list.getSelectionModel().getSelectedIndex();
            firstName.setText(String.valueOf(data.get(index).getFirstName()));
            LastName.setText(String.valueOf(data.get(index).getLastName()));
            contactNumber.setText(String.valueOf(data.get(index).getContactNumber()));
            data.remove(index);
            list.getItems().remove(index);
        }catch (Exception e){
        }
    }

    public void onFirst(ActionEvent actionEvent) throws IOException {
        if (data.isEmpty()){
            feedbackLabel.setText("You must set at least one (1) responder!");
        }else {
            BufferedWriter writer = null;
            try {
                writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "respondents.txt"), "utf-8"));

                int size = data.size();
                for (int i = 0; i < size; i++) {
                    writer.write(data.get(i).getFirstName());
                    writer.newLine();
                    writer.write(data.get(i).getLastName());
                    writer.newLine();
                    writer.write(data.get(i).getContactNumber());
                    writer.newLine();
                    writer.write("breakLine");
                    writer.newLine();
                }
                writer.write("-end-");
                writer.close();
            } catch (Exception e) {
            }
            feedbackLabel.setText("");
            changeScene changeScene = new changeScene();
            changeScene.setScene("Setup1.fxml", "style.css", actionEvent, "Fall Analyzer | Login Credentials");
        }
    }

    public void onSecond(ActionEvent actionEvent) throws IOException {
        if (data.isEmpty()){
            feedbackLabel.setText("You must set at least one (1) responder!");
        }else {
            BufferedWriter writer = null;
            try {
                writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "respondents.txt"), "utf-8"));

                int size = data.size();
                for (int i = 0; i < size; i++) {
                    writer.write(data.get(i).getFirstName());
                    writer.newLine();
                    writer.write(data.get(i).getLastName());
                    writer.newLine();
                    writer.write(data.get(i).getContactNumber());
                    writer.newLine();
                    writer.write("breakLine");
                    writer.newLine();
                }
                writer.write("-end-");
                writer.close();
            } catch (Exception e) {
            }
            feedbackLabel.setText("");
            changeScene changeScene = new changeScene();
            changeScene.setScene("Setup2.fxml", "style.css", actionEvent, "Fall Analyzer | Profile Setup");
        }
    }

    public void onThird(ActionEvent actionEvent) {
    }

    public void onFourth(ActionEvent actionEvent) throws IOException {
        if (data.isEmpty()){
            feedbackLabel.setText("You must set at least one (1) responder!");
        }else{
            BufferedWriter writer = null;
            try{
                writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "respondents.txt"), "utf-8"));

                int size = data.size();
                for (int i = 0; i < size; i++){
                    writer.write(data.get(i).getFirstName());
                    writer.newLine();
                    writer.write(data.get(i).getLastName());
                    writer.newLine();
                    writer.write(data.get(i).getContactNumber());
                    writer.newLine();
                    writer.write("breakLine");
                    writer.newLine();
                }
                writer.write("-end-");
                writer.close();
            } catch(Exception e){
            }
            feedbackLabel.setText("");
            changeScene changeScene = new changeScene();
            changeScene.setScene("Setup4.fxml", "style.css", actionEvent, "Fall Analyzer | Finish Setup");
        }

    }

    public void onFinish(ActionEvent actionEvent) {
    }
}
