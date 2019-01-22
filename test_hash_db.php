<?


$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

/*
// Connect to the DB
$con = mysqli_connect('localhost', 'user', 'pass', 'db'); 
// Escape the form values or user prepared statements
$username = mysqli_real_escape_string($con, $username);
$password = mysqli_real_escape_string($con, $password);
$sql = "SELECT * FROM users WHERE username = '".$username." AND password = '".$password."'";
$result = mysqli_query($con, $sql);
$count = mysqli_num_rows($result);
*/

$bill_no = $_POST['bill_no'];

$select = "SELECT bill_no
FROM cash_book
WHERE bill_no='".$bill_no."'";


$stmt = $dbh->prepare("$select");
$stmt -> execute();

//$row_array = $stmt->fetchAll();
$count = $stmt->rowCount();

if($count == 1) {
    echo "Success";
} else {
    echo "Fail";
}


?>