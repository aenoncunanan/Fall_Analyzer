package dlsu;

import dlsu.Utils.logInDialog;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.Group;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;

import java.io.*;
import java.net.URL;
import java.util.*;

public class exploreController implements Initializable{

    public Label usernameHello;
    public TextField usernameLogin;
    public PasswordField passwordLogin;
    public PasswordField confirmLogin;
    public Label feedbackLabelLogin;
    public Button editLogin;
    public Button saveLogin;

    public TextField firstNameProfile;
    public TextField LastNameProfile;
    public ChoiceBox genderProfile;
    public TextField ageProfile;
    public TextField addressProfile;
    public TextField contactNumberProfile;
    public Label feedbackLabel;
    public Button editProfile;
    public Button saveProfile;

    public Group editResponders;
    public TableView respondersTable;
    public TableColumn firstName;
    public TableColumn lastName;
    public TableColumn contactNumber;

    public TextField firstNameResponders;
    public TextField LastNameResponders;
    public TextField contactNumberResponders;
    public Button saveResponder;
    public Label allFieldsError;
    public Label successful;
    public Button deleteResponder;
    public Button editResponder;
    public Button addResponder;

    dlsu.Utils.logInDialog logInDialog = new logInDialog();

    public final ObservableList<responders> data = FXCollections.observableArrayList();

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
        initProfileTab();
        initRepondersTab();
        initActivityTab();
    }

    private void initActivityTab() {
        //insert here. if log file does not exist, do something.
//        BufferedReader br = null;
//        try {
//            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "log.txt"));
//            String temp = br.readLine();
//        } catch (IOException e) {
//        } finally {
//            try {
//                br.close();
//            } catch (IOException e) {
//            }
//        }
    }

    private void initRepondersTab(){
        //insert here. if respondents file does not exist, do something.

        BufferedReader br = null;
            try {
                br = new BufferedReader(new FileReader(selectCardController.driveLetter + "respondents.txt"));

                Boolean end = false;
                String line = "";
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
                        firstName.setCellValueFactory(new PropertyValueFactory<responders, String>("firstName"));
                        lastName.setCellValueFactory(new PropertyValueFactory<responders, String>("lastName"));
                        contactNumber.setCellValueFactory(new PropertyValueFactory<responders, String>("contactNumber"));
                        respondersTable.getItems().setAll(this.data);
                    }
                }
            } catch (IOException e) {
            } finally {
                try {
                    br.close();
                } catch (IOException e) {
                }
            }

    }

    private void initProfileTab() {
        //insert here. if login file does not exist, do something.

//        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
//        alert.setTitle("ALERT!");
//        String s = "Confirm to clear text in text field !";
//        alert.setContentText(s);
//
//        Optional<ButtonType> result = alert.showAndWait();
//
//        if ((result.isPresent()) && (result.get() == ButtonType.OK)) {
//            System.out.println("OK DAW");
//        }

        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));
            String temp = br.readLine();
            usernameHello.setText(temp + "!");
            usernameLogin.setText(temp);
            temp = br.readLine();
            passwordLogin.setText(temp);
            confirmLogin.setText(temp);
        } catch (IOException e) {
        } finally {
            try {
                br.close();
            } catch (IOException e) {
            }
        }

        genderProfile.getItems().add("male");
        genderProfile.getItems().add("female");
        try{
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "profile.txt"));
            firstNameProfile.setText(br.readLine());
            LastNameProfile.setText(br.readLine());
            genderProfile.setValue(br.readLine());
            ageProfile.setText(br.readLine());
            addressProfile.setText(br.readLine());
            contactNumberProfile.setText(br.readLine());
        } catch (IOException e){
        } finally {
            try {
                br.close();
            } catch (IOException e){
            }
        }
    }

    public void onEditLogin(ActionEvent actionEvent) {
        if(logInDialog.logIn()){
            usernameLogin.setEditable(true);
            passwordLogin.setEditable(true);
            confirmLogin.setEditable(true);
            saveLogin.setDisable(false);
            feedbackLabelLogin.setText("");
        }
    }

    public void onSaveLogin(ActionEvent actionEvent) {
        if(usernameLogin.getText().isEmpty() || passwordLogin.getText().isEmpty() || confirmLogin.getText().isEmpty()){
            feedbackLabelLogin.setText("All fields are required!");
        } else if (!passwordLogin.getText().equals(confirmLogin.getText())){
            feedbackLabelLogin.setText("Passwords do not match!");
        } else {
            usernameHello.setText(usernameLogin.getText() + "!");
            BufferedWriter writer = null;
            try {
                writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "login.txt"), "utf-8"));
                writer.write(usernameLogin.getText());
                writer.newLine();
                writer.write(passwordLogin.getText());
                writer.close();

                feedbackLabelLogin.setText("Login Credentials was successfully modified!");
                usernameLogin.setEditable(false);
                passwordLogin.setEditable(false);
                confirmLogin.setEditable(false);
                saveLogin.setDisable(true);
            } catch (Exception e) {
            }
        }

    }

    public void onEditProfile(ActionEvent actionEvent) {
        if(logInDialog.logIn()){
            firstNameProfile.setEditable(true);
            LastNameProfile.setEditable(true);
            genderProfile.setDisable(false);
            ageProfile.setEditable(true);
            addressProfile.setEditable(true);
            contactNumberProfile.setEditable(true);
            saveProfile.setDisable(false);
            feedbackLabel.setText("");
        }
    }

    public void onSaveProfile(ActionEvent actionEvent) {
        if(firstNameProfile.getText().isEmpty() || LastNameProfile.getText().isEmpty() || ageProfile.getText().isEmpty() || addressProfile.getText().isEmpty() || contactNumberProfile.getText().isEmpty()){
            feedbackLabel.setText("All fields are required!");
        } else {
            try {
                Integer.parseInt(ageProfile.getText());

                BufferedWriter writer = null;
                try {
                    writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "profile.txt"), "utf-8"));
                    writer.write(firstNameProfile.getText());
                    writer.newLine();
                    writer.write(LastNameProfile.getText());
                    writer.newLine();
                    writer.write(genderProfile.getValue().toString());
                    writer.newLine();
                    writer.write(ageProfile.getText());
                    writer.newLine();
                    writer.write(addressProfile.getText());
                    writer.newLine();
                    writer.write(contactNumberProfile.getText());
                    writer.close();

                    feedbackLabel.setText("Profile was successfully modified!");
                    firstNameProfile.setEditable(false);
                    LastNameProfile.setEditable(false);
                    genderProfile.setDisable(true);
                    ageProfile.setEditable(false);
                    addressProfile.setEditable(false);
                    contactNumberProfile.setEditable(false);
                    saveProfile.setDisable(true);
                } catch (Exception e) {
                }
            } catch (Exception e) {
                feedbackLabel.setText("Your age is invalid!");
            }
        }
    }

    public void saveResponder(){
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
    }

    public void onSaveResponder(ActionEvent actionEvent) {
        if (firstNameResponders.getText().isEmpty() || LastNameResponders.getText().isEmpty() || contactNumberResponders.getText().isEmpty()){
            allFieldsError.setText("All fields are required!");
            successful.setText("");
        } else {
            allFieldsError.setText("");

            data.add(new responders(firstNameResponders.getText(), LastNameResponders.getText(), contactNumberResponders.getText()));
            firstName.setCellValueFactory(new PropertyValueFactory<responders, String>("firstName"));
            lastName.setCellValueFactory(new PropertyValueFactory<responders, String>("lastName"));
            contactNumber.setCellValueFactory(new PropertyValueFactory<responders, String>("contactNumber"));
            respondersTable.getItems().setAll(this.data);

            firstNameResponders.setText("");
            LastNameResponders.setText("");
            contactNumberResponders.setText("");

            successful.setText("Responders were successfully modified!");
            editResponders.setDisable(true);
            saveResponder();
        }
    }

    public void onDeleteResponder(ActionEvent actionEvent) {
        int selectedIndex = respondersTable.getSelectionModel().getSelectedIndex();
        if (selectedIndex >= 0){
            if (logInDialog.logIn()){
                respondersTable.getItems().remove(selectedIndex);
                data.remove(selectedIndex);
                allFieldsError.setText("");
                successful.setText("Responders were successfully modified!");
                saveResponder();
            }
        }
    }

    public void onEditResponder(ActionEvent actionEvent) {
        if(logInDialog.logIn()){
            int selectedIndex = respondersTable.getSelectionModel().getSelectedIndex();
            if (selectedIndex >= 0){
                respondersTable.getItems().remove(selectedIndex);
                firstNameResponders.setText(data.get(selectedIndex).getFirstName());
                LastNameResponders.setText(data.get(selectedIndex).getLastName());
                contactNumberResponders.setText(data.get(selectedIndex).getContactNumber());
                data.remove(selectedIndex);
            }

            allFieldsError.setText("");
            successful.setText("");
            editResponders.setDisable(false);
        }
    }

    public void onAddResponder(ActionEvent actionEvent) {
        if (logInDialog.logIn()){
            allFieldsError.setText("");
            successful.setText("");
            editResponders.setDisable(false);
        }
    }

}
