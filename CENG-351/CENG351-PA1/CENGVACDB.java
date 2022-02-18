package ceng.ceng351.cengvacdb;
import ceng.ceng351.cengvacdb.*;
import java.sql.*;
import java.util.ArrayList;

public class CENGVACDB implements ICENGVACDB {

    static String username = "e2309839";
    static String password = "zXrxUl$T*sEy";
    static String host = "144.122.71.121";
    static String database = "db2309839";
    static int port = 8080;
    static Connection connection = null;

    @Override
    public void initialize() {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database + "?useSSL=false";

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection =  DriverManager.getConnection(url, username, password);
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int createTables() {

        String createUser = "CREATE TABLE IF NOT EXISTS User ("
                + "userID INTEGER NOT NULL,"
                + "userName CHAR(30),"
                + "age INTEGER,"
                + "address CHAR(150),"
                + "password CHAR(30),"
                + "status CHAR(15),"
                + "PRIMARY KEY (userID))";

        String createVaccine = "CREATE TABLE IF NOT EXISTS Vaccine ("
                + "code INTEGER NOT NULL,"
                + "vaccinename CHAR(30),"
                + "type CHAR(30),"
                + "PRIMARY KEY (code))";

        String createVaccination = "CREATE TABLE IF NOT EXISTS Vaccination ("
                + "code INTEGER NOT NULL,"
                + "userID INTEGER NOT NULL,"
                + "dose INTEGER NOT NULL,"
                + "vacdate DATE,"
                + "FOREIGN KEY (code) REFERENCES Vaccine(code),"
                + "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE CASCADE ON UPDATE CASCADE,"
                + "PRIMARY KEY (code, userID, dose))";

        String createAllergicSideEffect = "CREATE TABLE IF NOT EXISTS AllergicSideEffect ("
                + "effectcode INTEGER NOT NULL,"
                + "effectname CHAR(50),"
                + "PRIMARY KEY (effectcode))";

        String createSeen = "CREATE TABLE IF NOT EXISTS Seen ("
                + "effectcode INTEGER NOT NULL,"
                + "code INTEGER NOT NULL,"
                + "userID INTEGER NOT NULL,"
                + "date DATE,"
                + "degree CHAR(30),"
                + "FOREIGN KEY (effectcode) REFERENCES AllergicSideEffect(effectcode),"
                + "FOREIGN KEY (code) REFERENCES Vaccine(code) ON DELETE CASCADE,"
                + "FOREIGN KEY (userID) REFERENCES User(userID),"
                + "PRIMARY KEY (effectcode, code, userID))";


        int count = 0;

        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(createUser);
            count++;

            statement.executeUpdate(createVaccine);
            count++;

            statement.executeUpdate(createVaccination);
            count++;

            statement.executeUpdate(createAllergicSideEffect);
            count++;

            statement.executeUpdate(createSeen);
            count++;

            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public int dropTables() {
        String dropUser = "DROP TABLE IF EXISTS User";
        String dropVaccine = "DROP TABLE IF EXISTS Vaccine ";
        String dropVaccination = "DROP TABLE IF EXISTS Vaccination";
        String dropAllergicSideEffect = "DROP TABLE IF EXISTS AllergicSideEffect";
        String dropSeen = "DROP TABLE IF EXISTS Seen";

        int count = 0;
        try
        {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(dropSeen);
            count++;

            statement.executeUpdate(dropAllergicSideEffect);
            count++;

            statement.executeUpdate(dropVaccination);
            count++;

            statement.executeUpdate(dropVaccine);
            count++;

            statement.executeUpdate(dropUser);
            count++;
            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }
        return count;
    }

    @Override
    public int insertUser(User[] users) {

        int count = 0;
        String insertUser = "INSERT INTO User(" +
                "userID, " +
                "userName, " +
                "age, " +
                "address, " +
                "password, " +
                "status) VALUES (?, ?, ?, ?, ?, ?)";

        for(int i = 0; i < users.length; i++) {
            User temp = users[i];
            try
            {
                PreparedStatement statement = this.connection.prepareStatement(insertUser);

                statement.setInt(1, temp.getUserID());
                statement.setString(2, temp.getUserName());
                statement.setInt(3, temp.getAge());
                statement.setString(4, temp.getAddress());
                statement.setString(5, temp.getPassword());
                statement.setString(6, temp.getStatus());

                statement.executeUpdate();
                count++;
                statement.close();
            } catch(SQLException e)
            {
                e.printStackTrace();
            }
        }


        return count;
    }

    @Override
    public int insertAllergicSideEffect(AllergicSideEffect[] sideEffects) {

        int count = 0;
        String insertAllergicSideEffect = "INSERT INTO AllergicSideEffect(" +
                "effectcode, " +
                "effectname) VALUES (?, ?)";
        try
        {
            PreparedStatement statement = this.connection.prepareStatement(insertAllergicSideEffect);
            for(int i = 0; i < sideEffects.length; i++){
                AllergicSideEffect temp = sideEffects[i];
                statement.setInt(1, temp.getEffectCode());
                statement.setString(2, temp.getEffectName());

                statement.executeUpdate();
                count++;
            }
            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public int insertVaccine(Vaccine[] vaccines) {

        int count = 0;
        String insertVaccine = "INSERT INTO Vaccine(" +
                "code, " +
                "vaccinename, " +
                "type) VALUES (?, ?, ?)";
        try
        {
            PreparedStatement statement = this.connection.prepareStatement(insertVaccine);
            for(int i = 0; i < vaccines.length; i++){
                Vaccine vaccine = vaccines[i];
                statement.setInt(1, vaccine.getCode());
                statement.setString(2, vaccine.getVaccineName());
                statement.setString(3, vaccine.getType());

                statement.executeUpdate();
                count++;
            }

            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public int insertVaccination(Vaccination[] vaccinations) {
        int count = 0;

        String insertVaccination = "INSERT INTO Vaccination(" +
                "code, " +
                "userID, " +
                "dose, " +
                "vacdate) VALUES (?, ?, ?, ?)";
        try
        {
            PreparedStatement statement = this.connection.prepareStatement(insertVaccination);
            for(int i = 0; i < vaccinations.length; i++){
                Vaccination temp = vaccinations[i];
                statement.setInt(1, temp.getCode());
                statement.setInt(2, temp.getUserID());
                statement.setInt(3, temp.getDose());
                statement.setString(4, temp.getVacdate());

                statement.executeUpdate();
                count++;
            }
            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        return count;
    }

    @Override
    public int insertSeen(Seen[] seens) {

        int count = 0;
        String insertSeen = "INSERT INTO Seen(" +
                "effectcode, " +
                "code, " +
                "userID, " +
                "date, " +
                "degree) VALUES (?, ?, ?, ?, ?)";
        try
        {
            PreparedStatement statement = this.connection.prepareStatement(insertSeen);
            for(int i = 0; i < seens.length; i++){
                Seen seen = seens[i];
                statement.setInt(1, seen.getEffectcode());
                statement.setInt(2, seen.getCode());
                statement.setString(3, seen.getUserID());
                statement.setString(4, seen.getDate());
                statement.setString(5, seen.getDegree());

                statement.executeUpdate();
                count++;
            }

            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }

        return count;

    }
    /*
    User (--userID:int,
    userName:varchar(30),
    age:int,
    address:varchar(150),
    password:varchar(30),
    status:varchar(15))

    Vaccine (--code:int,
    vaccinename:varchar(30),
    type:varchar(30))

    Vaccination (--code:int Vaccine,
    --userID:int User,
    --dose:int,
    vacdate:date)

    AllergicSideEffect (--effectcode:int,
    effectname:varchar(50))

    Seen (--effectcode:int AllergicSideEffect,
    --code:int Vaccination,
    --userID:int User,
    date:date,
    degree:varchar(30))
     */

    @Override
    public Vaccine[] getVaccinesNotAppliedAnyUser() {

        String query3 = "SELECT DISTINCT V.code, V.vaccinename, V.type " +
                "FROM Vaccine V " +
                "WHERE V.code NOT IN " +
                "(SELECT DISTINCT Vac.code FROM Vaccination Vac) " +
                "ORDER BY V.code ASC;";

        ArrayList<Vaccine> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query3);

            while(res.next()){
                int code = res.getInt("code");
                String vaccinename = res.getString("vaccinename") ;
                String type = res.getString("type");
                tmp_array.add(new Vaccine(
                        code, vaccinename, type));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        Vaccine[] result_array =
                new Vaccine[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersforTwoDosesByDate(String vacdate) {
        String query4 = "SELECT DISTINCT U.userID, U.userName, U.address " +
                "FROM Vaccination Vac, User U " +
                "WHERE U.userID = Vac.userID "+
                "AND Vac.dose = 1 "+
                "AND Vac.vacdate >= \""+ vacdate +
                "\" AND U.userID IN "+
                "(SELECT DISTINCT U2.userID " +
                "FROM User U2, Vaccination V2 " +
                "WHERE U2.userID = V2.userID AND  V2.dose = 2) " +
                "ORDER BY U.userID ASC ";

        ArrayList<QueryResult.UserIDuserNameAddressResult> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query4);

            while(res.next()){
                String userID = res.getString("userID");
                String userName = res.getString("userName") ;
                String address = res.getString("address");
                tmp_array.add(new QueryResult.UserIDuserNameAddressResult(
                        userID, userName, address));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.UserIDuserNameAddressResult[] result_array =
                new QueryResult.UserIDuserNameAddressResult[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array;
    }
    /*
    List of the two recent vaccines applied to any user that do
not contain ‘vac’ in their name (10 pts)
Output: code, vaccinename, type
Order the results by code in ascending order
     */
    @Override
    public Vaccine[] getTwoRecentVaccinesDoNotContainVac() {
        String query5 = "SELECT DISTINCT V.code, V.vaccinename, V.type " +
                "FROM Vaccine V " +
                "WHERE V.code NOT IN " +
                "(SELECT DISTINCT Vac.code FROM Vaccination Vac) " +
                "ORDER BY V.code ASC;";

        ArrayList<Vaccine> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query5);

            while(res.next()){
                int code = res.getInt("code");
                String vaccinename = res.getString("vaccinename") ;
                String type = res.getString("type");
                tmp_array.add(new Vaccine(
                        code, vaccinename, type));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        Vaccine[] result_array =
                new Vaccine[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersAtHasLeastTwoDoseAtMostOneSideEffect() {

        String query6 = "SELECT DISTINCT U.userID, U.userName, U.address " +
                "FROM Vaccination Vac1, Vaccination Vac2, User U " +
                "WHERE U.userID = Vac1.userID AND  U.UserID = Vac2.userID " +
                "AND (Vac1.code != Vac2.code OR Vac1.dose != Vac2.dose) " +
                "AND U.userID NOT IN " +
                "(SELECT DISTINCT U2.userID " +
                "FROM User U2, Seen S1, Seen S2 " +
                "WHERE U2.userID = S1.userID AND U2.userID = S2.userID " +
                "AND (S1.effectcode != S2.effectcode OR S1.code != S2.code)) " +
                "ORDER BY U.userID ASC ";


        ArrayList<QueryResult.UserIDuserNameAddressResult> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query6);

            while(res.next()){
                String userID = res.getString("userID");
                String userName = res.getString("userName") ;
                String address = res.getString("address");
                tmp_array.add(new QueryResult.UserIDuserNameAddressResult(
                        userID, userName, address));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.UserIDuserNameAddressResult[] result_array =
                new QueryResult.UserIDuserNameAddressResult[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersWithAllVaccinesCanCauseGivenSideEffect(String effectname) {

        String query7 =  "SELECT DISTINCT U.userID, U.userName, U.address " +
                "FROM User U, Vaccination V " +
                "WHERE U.userID = V.userID  AND NOT EXISTS " +
                "(SELECT DISTINCT V.code "+
                "FROM Vaccination V, Seen S " +
                "WHERE V.userID = U.userID AND  S.code = V.code AND S.code NOT IN " +
                "(SELECT DISTINCT S.code " +
                "FROM AllergicSideEffect A " +
                "WHERE S.effectcode = A.effectcode AND A.effectname = \""+ effectname +
                "\" ))" +
                "ORDER BY U.userID ASC ";

        ArrayList<QueryResult.UserIDuserNameAddressResult> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query7);

            while(res.next()){
                String userID = res.getString("userID");
                String userName = res.getString("userName") ;
                String address = res.getString("address");
                tmp_array.add(new QueryResult.UserIDuserNameAddressResult(
                        userID, userName, address));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.UserIDuserNameAddressResult[] result_array =
                new QueryResult.UserIDuserNameAddressResult[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersWithAtLeastTwoDifferentVaccineTypeByGivenInterval(String startdate, String enddate) {

        String query8 = "SELECT DISTINCT U.userID, U.userName, U.address " +
                "FROM User U, Vaccine V1, Vaccination Vac, Vaccine V2, Vaccination Vac2 " +
                "WHERE U.userID = Vac.userID AND U.userID = Vac2.userID AND " +
                "Vac.code = V1.code AND " +
                "Vac2.code = V2.code AND " +
                "V1.code != V2.code AND " +
                "V1.type != V2.type AND " +
                "Vac.vacdate >= \""+ startdate +
                "\" AND " +
                "Vac.vacdate <= \""+ enddate +
                "\" " +
                "ORDER BY U.userID ASC ";

        ArrayList<QueryResult.UserIDuserNameAddressResult> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query8);

            while(res.next()){
                String userID = res.getString("userID");
                String userName = res.getString("userName") ;
                String address = res.getString("address");
                tmp_array.add(new QueryResult.UserIDuserNameAddressResult(
                        userID, userName, address));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        QueryResult.UserIDuserNameAddressResult[] result_array =
                new QueryResult.UserIDuserNameAddressResult[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array;

    }

    @Override
    public AllergicSideEffect[] getSideEffectsOfUserWhoHaveTwoDosesInLessThanTwentyDays() {
        String query9 = "SELECT DISTINCT A.effectcode, A.effectname " +
        "FROM Vaccination V, AllergicSideEffect A, User U, Seen S " +
        "WHERE U.userID = V.userID AND V.userID = S.userID AND S.effectcode = A.effectcode AND EXISTS ( " +
        "SELECT DISTINCT 1 FROM User AS U1, Vaccination V1, Vaccination V2 " +
        "WHERE (U1.userID = V1.userID AND U1.userID = V2.userID AND V1.dose = 1 " +
        "AND V2.dose = 2 AND DATEDIFF(V2.vacdate, V1.vacdate) <= 20) AND (U.userID = U1.userID) " +
        ") ORDER BY A.effectcode ASC ";

        ArrayList<AllergicSideEffect> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query9);

            while(res.next()){
                int effectcode = res.getInt("effectcode");
                String effectname = res.getString("effectname") ;
                tmp_array.add(new AllergicSideEffect(
                        effectcode, effectname));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        AllergicSideEffect[] result_array =
                new AllergicSideEffect[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array;
    }

    @Override
    public double averageNumberofDosesofVaccinatedUserOverSixtyFiveYearsOld() {
        String query10 = "SELECT DISTINCT AVG(lastdose) AS avgdose " +
                "FROM " +
                "(SELECT MAX(Vac.dose) AS lastdose, U.userID FROM User U, Vaccination Vac " +
                "WHERE Vac.userID = U.userID AND U.age > 65 " +
                "GROUP BY U.userID) AS own_alias;";

        ResultSet res;
        double result = 0;
        int total_dose = 0;
        int max_dose = 0;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query10);

            while(res.next()){
                //total_dose = res.getInt("totaldose");
                //max_dose = res.getInt("maxdose");
                result = res.getDouble("avgdose");
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }
        return result;
    }

    @Override
    public int updateStatusToEligible(String givendate) {

        String query11 = "UPDATE User " +
                "SET status = 'Eligible' WHERE status = 'Not_Eligible' " +
                "AND userID IN (SELECT Vac.userID " +
                "FROM Vaccination Vac WHERE userID = Vac.userID " +
                "GROUP BY Vac.userID HAVING DATEDIFF( \"" + givendate + "\" ,MAX(Vac.vacdate)) > 120) ";

        int number_of_updates = 0;

        try{
            Statement statement = this.connection.createStatement();
            number_of_updates = statement.executeUpdate(query11);

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        return number_of_updates;
    }

    @Override
    public Vaccine deleteVaccine(String vaccineName) {
        String query12 = "SELECT DISTINCT V.code, V.vaccineName, V.type " +
                "FROM Vaccine V WHERE V.vaccineName = \""+ vaccineName +
                "\" ";
        String query12_delete = "DELETE FROM Vaccine WHERE vaccineName = ? ";
        ArrayList<Vaccine> tmp_array = new ArrayList<>();
        ResultSet res;

        try{
            Statement statement = this.connection.createStatement();
            res = statement.executeQuery(query12);

            while(res.next()){
                int code = res.getInt("code");
                String vaccinename = res.getString("vaccinename") ;
                String type = res.getString("type");
                tmp_array.add(new Vaccine(
                        code, vaccinename, type));
            }

            statement.close();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        Vaccine[] result_array =
                new Vaccine[tmp_array.size()];

        for(int i = 0; i < tmp_array.size(); i++){
            result_array[i] = tmp_array.get(i);
        }

        return result_array[0];
    }
}