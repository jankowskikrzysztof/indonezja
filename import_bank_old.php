<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

$row = 1;
if (($handle = fopen("banco/TRX_INQ-SCUBAFROGGY-2017022803001500853.csv", "r")) !== FALSE) {
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
        $num = count($data);
        //echo "<p> $num fields in line $row: <br /></p>\n";
        $row++;
        //for ($c=0; $c < $num; $c++) {
        //    echo $c.': '.$data[$c] . "<br />\n";
        //}

    if($data[0]<>'Account No')
      {
      $insert = 'INSERT INTO bank_statement
                  ( `account_no`
                   ,`post_date`
                   ,`value_date`
                   ,`branch`
                   ,`journal_no`
                   ,`description`
                   ,`debit`
                   ,`credit`
                   )
                  VALUES
                  ("'.$data[0].'"
                  ,"20'.substr($data[1],6,2).'-'.substr($data[1],3,2).'-'.substr($data[1],0,2).'"
                  ,"20'.substr($data[2],6,2).'-'.substr($data[2],3,2).'-'.substr($data[2],0,2).'"
                  ,"'.$data[3].'"
                  ,"'.$data[4].'"
                  ,"'.$data[5].'"
                  ,"'.str_replace(array(',','.00'),'',$data[6]).'"
                  ,"'.str_replace(array(',','.00'),'',$data[7]).'"
                  )';
      echo $insert.'<br>';

      $stmt = $dbh->prepare($insert);
      $stmt -> execute();

      }

    }
    fclose($handle);
}



?>


<?php include('inc/footer.inc'); ?>