package dlsu.Utils;

/**
 * Created by aenon on 25/11/2017.
 */
public class checkContactNumber {
    public static Boolean validContactNumber(String contactNumber){
        if (contactNumber.length() > 13){
            return false;
        } else{
            String num = contactNumber.substring(1);

            try{
                Long.parseLong(num);
                if (num.length() != 12){
                    return false;
                } else {
                    return true;
                }
            } catch (NumberFormatException e){
                return false;
            }
        }
    }
}
