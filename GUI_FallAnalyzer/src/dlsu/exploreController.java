package dlsu;

import dlsu.Utils.logInDialog;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.Group;
import javafx.scene.chart.PieChart;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import java.io.*;
import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
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

    public TableView activityTable;
    public TableColumn dateCell;
    public TableColumn timeCell;
    public TableColumn activityCell;
    public TableColumn locationCell;
    public Label feedbackData;
    public Button deleteData;
    public Button deleteAllData;
    public Button exportData;
    public Label numberOfFalls;
    public PieChart storageSpace;

    public int fallTimes = 0;
    public Tab profileTab;
    public Tab respondersTab;
    public Tab activityTab;

    dlsu.Utils.logInDialog logInDialog = new logInDialog();

    public final ObservableList<responders> data = FXCollections.observableArrayList();
    public final ObservableList<activities> log = FXCollections.observableArrayList();

    public static class activities{
        private SimpleStringProperty date;
        private SimpleStringProperty time;
        private SimpleStringProperty activity;
        private SimpleStringProperty location;

        private activities(String dateLog, String timeLog, String activityLog, String locationLog){
            this.date = new SimpleStringProperty(dateLog);
            this.time = new SimpleStringProperty(timeLog);
            this.activity = new SimpleStringProperty(activityLog);
            this.location = new SimpleStringProperty(locationLog);
        }

        public String getDate() {
            return date.get();
        }

        public void setDate(String dateLog) {
            date.set(dateLog);
        }

        public String getTime() {
            return time.get();
        }

        public void setTime(String timeLog) {
            time.set(timeLog);
        }

        public String getActivity() {
            return activity.get();
        }

        public void setActivity(String activityLog) {
            activity.set(activityLog);
        }

        public String getLocation() {
            return location.get();
        }

        public void setLocation(String locationLog) {
            location.set(locationLog);
        }
    }

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
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "activity.txt"));
            String line = "";
            String date = "";
            String time = "";
            String activity = "";
            String location = "";

            while((line = br.readLine()) != null){
                if (line.equals("-end_of_activity-")){
                    //do nothing
                } else {
                    date = line;
                    time = br.readLine();
                    activity = br.readLine();
                    if(activity.equals("Falling!")){
                        fallTimes++;
                        numberOfFalls.setText(String.valueOf(fallTimes));
                    }
                    location = br.readLine();
                    log.add(new activities(date, time, activity, location));
                    dateCell.setCellValueFactory(new PropertyValueFactory<activities, String>("date"));
                    timeCell.setCellValueFactory(new PropertyValueFactory<activities, String>("time"));
                    activityCell.setCellValueFactory(new PropertyValueFactory<activities, String>("activity"));
                    locationCell.setCellValueFactory(new PropertyValueFactory<activities, String>("location"));
                    activityTable.getItems().setAll(this.log);
                }
            }

        } catch (IOException e) {
            System.out.println(e);
        } finally {
            try {
                br.close();
            } catch (IOException e) {
                System.out.println(e);
            }
        }

        setStorageChart();
    }

    private void initRepondersTab(){
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "respondents.txt"));

            Boolean end = false;
            String line = "";
            String first = "";
            String last = "";
            String contact = "";
            while (end == false) {
                line = br.readLine();
                if (line.equals("-end-")) {
                    end = true;
                } else if (line.equals("breakLine")) {
                    //just do nothing
                } else {
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
        try {
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "profile.txt"));
            firstNameProfile.setText(br.readLine());
            LastNameProfile.setText(br.readLine());
            genderProfile.setValue(br.readLine());
            ageProfile.setText(br.readLine());
            addressProfile.setText(br.readLine());
            contactNumberProfile.setText(br.readLine());
        } catch (IOException e) {
        } finally {
            try {
                br.close();
            } catch (IOException e) {
            }
        }

    }

    private void setStorageChart() {
        File space = new File(selectCardController.driveLetter);
        long freespace = space.getFreeSpace() / (1024*1024*1024); //outputs in GB. (1024*1024) if MB
        long totalspace = space.getTotalSpace() / (1024*1024*1024); //outputs  in*GB (1024*1024) if MB
        long usedspace = (space.getTotalSpace() - space.getFreeSpace()) / (1024*1024*1024); //outputs  in*GB (1024*1024) if MB

        ObservableList<PieChart.Data> pieDate =
                FXCollections.observableArrayList(
                        new PieChart.Data("Free Space: " + freespace + " GB", freespace),
                        new PieChart.Data("Used Space: " + usedspace + " GB", usedspace)
                );
        storageSpace.setData(pieDate);
        storageSpace.setLabelLineLength(10);
        storageSpace.setLabelsVisible(false);

        storageSpace.getData().stream().forEach(data -> {
            Tooltip tooltip = new Tooltip();
            tooltip.setText(String.format("%.1f%%", 100*data.getPieValue()/totalspace));
            Tooltip.install(data.getNode(), tooltip);
            data.pieValueProperty().addListener((observable, oldValue, newValue) ->
                    tooltip.setText(newValue + "%"));
        });
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
                setStorageChart();
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
                    setStorageChart();
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
            setStorageChart();
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

    public void saveLog(){
        BufferedWriter writer = null;
        try{
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "activity.txt"), "utf-8"));

            int size = log.size();
            for (int i = 0; i < size; i++){
                writer.write(log.get(i).getDate());
                writer.newLine();
                writer.write(log.get(i).getTime());
                writer.newLine();
                writer.write(log.get(i).getActivity());
                writer.newLine();
                writer.write(log.get(i).getLocation());
                writer.newLine();
                writer.write("-end_of_activity-");
                writer.newLine();
            }
            writer.close();
            setStorageChart();
        } catch(Exception e){
        }
    }

    public void onDeleteData(ActionEvent actionEvent){
        int selectedIndex = activityTable.getSelectionModel().getSelectedIndex();
        if (selectedIndex >= 0){
            if (logInDialog.logIn()){
                if(activityCell.getCellObservableValue(selectedIndex).getValue().equals("Falling!")){
                    fallTimes--;
                    numberOfFalls.setText(String.valueOf(fallTimes));
                }
                activityTable.getItems().remove(selectedIndex);
                log.remove(selectedIndex);
                feedbackData.setText("Data log was successfully modified!");
                saveLog();
            }
        }

    }

    public void onDeleteAllData(ActionEvent actionEvent) {
        if (logInDialog.logIn()){
            fallTimes = 0;
            numberOfFalls.setText(String.valueOf(fallTimes));
            activityTable.getItems().clear();
            log.clear();
            feedbackData.setText("Data log was successfully modified!");
            saveLog();
        }
    }

    public void onExportData(ActionEvent actionEvent) throws IOException {
        HSSFWorkbook workbook = new HSSFWorkbook();
        HSSFSheet sheet = workbook.createSheet("Fall Analyzer Data Log");

        HSSFRow row = null;
        HSSFCell cell = null;

        row = sheet.createRow(0);
        cell = row.createCell(0);
        cell.setCellValue("DATE");
        cell = row.createCell(1);
        cell.setCellValue("TIME");
        cell = row.createCell(2);
        cell.setCellValue("ACTIVITY");
        cell = row.createCell(3);
        cell.setCellValue("LOCATION");

        row = sheet.createRow(1);
        cell = row.createCell(0);
        cell.setCellValue("try");

        row = sheet.createRow(2);
        cell = row.createCell(1);
        cell.setCellValue("tr2y");

        for (int rowIterate = 0; rowIterate < activityTable.getItems().size(); rowIterate++){
            row = sheet.createRow(rowIterate+1);
            for (int columnIterate = 0; columnIterate < activityTable.getColumns().size(); columnIterate++){
                cell = row.createCell(columnIterate);
                switch (columnIterate){
                    case 0: //DATE
                        cell.setCellValue(dateCell.getCellObservableValue(rowIterate).getValue().toString());
                        break;
                    case 1: //TIME
                        cell.setCellValue(timeCell.getCellObservableValue(rowIterate).getValue().toString());
                        break;
                    case 2: //ACTIVITY
                        cell.setCellValue(activityCell.getCellObservableValue(rowIterate).getValue().toString());
                        break;
                    case 3: //LOCATION
                        cell.setCellValue(locationCell.getCellObservableValue(rowIterate).getValue().toString());
                        break;
                }
            }
        }

        sheet.autoSizeColumn(0);
        sheet.autoSizeColumn(1);
        sheet.autoSizeColumn(2);
        sheet.autoSizeColumn(3);

        DateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_HHmmss");
        Date date = new Date();
        String fileName = "Fall Analyzer Data Log " + dateFormat.format(date) + ".xls";

        workbook.write(new FileOutputStream(fileName));
        workbook.close();

        feedbackData.setText("Data log was successfully exported!");
    }
}
