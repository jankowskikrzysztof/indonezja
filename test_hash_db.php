<?php


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

$bill_no = $_POST['email'];

$stmt = $dbh->prepare("SELECT bill_no FROM cash_book WHERE bill_no='".$bill_no."'");
$stmt -> execute();

$rows = $stmt->fetchAll();
$num_rows = count($rows);

//echo $num_rows;

//  if($num_rows > 0) {
//      //return true;
//      echo 'true';
//  } else {
//      //return false;
//      echo 'false';
//  }

 if( $num_rows > 0 ){
    echo json_encode(array("msg"=>"Already exists"));
} else {
    echo json_encode(array("msg"=>"Not exists"));
}


?>