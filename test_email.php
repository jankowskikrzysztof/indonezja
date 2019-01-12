<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');


use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

require 'inc/PHPMailer/src/Exception.php';
require 'inc/PHPMailer/src/PHPMailer.php';
require 'inc/PHPMailer/src/SMTP.php';

$smtpUsername = 'nocnymark@gmail.com';
$smtpPassword = '0696447614';


$mail = new PHPMailer;
$mail->isSMTP();
$mail->SMTPDebug = 0; // 0 = off (for production use) - 1 = client messages - 2 = client and server messages
$mail->Host = "smtp.gmail.com"; // use $mail->Host = gethostbyname('smtp.gmail.com'); // if your network does not support SMTP over IPv6
$mail->Port = 587; // TLS only
$mail->SMTPSecure = 'tls'; // ssl is depracated
$mail->SMTPAuth = true;
$mail->Username = $smtpUsername;
$mail->Password = $smtpPassword;


$trip = $_POST['sel_trip'];
$emailTo = $_POST['email'];

$stmt = $dbh->prepare("SELECT date_from, date_to, txt_route, id_boat_trip as boat_trip_id
                              FROM boat_trip
                              WHERE boat_trip.id_boat_trip=".$trip."
                              ORDER BY date_to");
$stmt -> execute();
$boat_trip_array = $stmt->fetchAll();
                     foreach($boat_trip_array as $row)
                        {
                        $trip_name = $row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'];
                        }


$subject = 'Clients list: '.$trip_name;


 $message_naglowek = '
  <html>
  <body>
    <h1>'.$trip_name.'</h1>
    <table border="1" style="border-collapse: collapse; border: 1px;" cellspacing="3" width="100%">
   <thead>
    <tr align=center bgcolor=c0c0c0>
     <th>Name</th>
     <th>Date of born</th>
     <th>Gender</th>
     <th>Nationality</th>
     <th>No of dives</th>
     <th>Passport</th>
     <th>Buyer</th>
     <th>Nitrox</th>
     <th>Gear to rent</th>
     <th>Remarks</th>
     <th>email</th>
     <th>Park fee</th>
  </tr>
 </thead>';


$stmt = $dbh->prepare("SELECT client.*
                              FROM client
                              left join cash_book on cash_book.client_id=client.id_client
                              WHERE cash_book.boat_trip_id=".$trip."
                              GROUP BY client.id_client
                              ORDER BY name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $message_tresc .= '<tr>
   <td>'.$row['name'].'</td>
   <td>'.$row['date_of_born'].'</td>
   <td>'.$row['gender'].'</td>
   <td>'.$row['nationality'].'</td>
   <td>'.$row['no_of_dives'].'</td>
   <td>'.$row['passport'].'</td>
   <td>'.$row['buyer'].'</td>
   <td>'.$row['nitrox'].'</td>
   <td>'.$row['gear_to_rent'].'</td>
   <td>'.$row['remarks'].'</td>
   <td><a href="mailto:'.$row['email'].'">'.$row['email'].'</a></td>
   <td>'.$row['park_fee'].'</td>
   </tr>';


   }


  $message_stopka = '
</table>
  </body>
  </html>';



//mail($emaile, $subject, $message_naglowek.$message_tresc.$message_stopka, $headers);



//echo '<p>Mail send OK</p>';

$emailFromName = 'SCUBA';
$emailFrom = 'nocnymark@gmail.com';

$mail->setFrom($emailFrom, $emailFromName);
$mail->addAddress($emailTo, $emailToName);
$mail->Subject = $subject;
$mail->msgHTML($message_naglowek.$message_tresc.$message_stopka); //$mail->msgHTML(file_get_contents('contents.html'), __DIR__); //Read an HTML message body from an external file, convert referenced images to embedded,
$mail->AltBody = 'HTML messaging not supported';
// $mail->addAttachment('images/phpmailer_mini.png'); //Attach an image file

if(!$mail->send()){
    $result = "Mailer Error: " . $mail->ErrorInfo;
}else{
    $result = "Message sent OK!";
}

echo '<h2>'.$result.'</h2>';

$stmt = $dbh->prepare("INSERT INTO `log_email`
                                  (`sender`,`emails`,`subject`,`message`, `result`)
                           VALUES ('".$_SESSION[user_id]."','".$emailTo."','".$subject."','".$message_naglowek.$message_tresc.$message_stopka."','".$result."')
                           ");
$stmt -> execute();


?>

<?php include('inc/footer.inc'); ?>