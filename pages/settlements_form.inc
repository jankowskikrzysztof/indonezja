<?php


$id = $_GET['id'];


if($action=='add')
                   {
                    $foreign_table = $_GET['foreign_table'];
                    $foreign_id = $_GET['foreign_id'];
                    $value = $_GET['value'];
                    $date = $_GET['date'];
                    $desc = $_GET['desc'];
                    $acc_filter = $_GET['acc_filter'];
                    $client = $_GET['client'];
                    if($acc_filter<>'')
                        $acc_filter_sql = 'and bank_statement.account_no='.$acc_filter;
                    else    
                        $acc_filter_sql = '';
                    }

if($_GET['bookers']==0)
    {
    $exclude_transfers = "and `description` NOT LIKE '%PADI TRAVEL%' 
                        and `description` NOT LIKE '%LIVEABOARD.COM%' 
                        and `description` NOT LIKE '%DIVEBOOKER.COM%'
                        and `description` NOT LIKE '%DIVIAC AG%'
                        ";
    }
else   
    $exclude_transfers = '';


$bs_sql = "SELECT 
                    (CASE WHEN debit<>0 THEN debit WHEN credit<>0 THEN credit END) as value,
                    bank_statement.*,
                    (bank_statement.credit+bank_statement.debit)-SUM(settlements.value) as still_to_settle
                    FROM bank_statement
                    left join settlements on settlements.bank_statement_id=bank_statement.id_bank_statement
                    where (settlements.bank_statement_id IS NULL or settlements.value<>(bank_statement.credit+bank_statement.debit))
                    $acc_filter_sql
                    $exclude_transfers
                    GROUP BY bank_statement.id_bank_statement
                    ORDER BY bank_statement.account_no, bank_statement.value_date DESC";

//echo $bs_sql;

$stmt = $dbh->prepare("$bs_sql");
$stmt -> execute();
$bank_statement_array = $stmt->fetchAll();



?>



<div>

<h2>Settlements</h2>

<form action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >



            <!-- <div class="pure-control-group">
                <label for="name">Foreign table</label> -->
                <input type="hidden" name="foreign_table" id="foreign_table" value="<?php echo $foreign_table; ?>">
                <!-- <?php echo $foreign_table; ?>
            </div> -->

            <div class="pure-control-group">
            <label for="name">Document date: <b><?php echo $date; ?></b></label>
            </div>

            <div class="pure-control-group">
            <label for="name">Document value: <b><?php echo $formatter->formatCurrency($value, 'IDR'); ?></b></label>
            </div>

            <div class="pure-control-group">
            <label for="name">Document description: <b><?php echo $desc; ?></b></label>
            </div>

            <div class="pure-control-group">
            <label for="name">Document client: <b><?php echo $client; ?></b></label>
            </div>


            <!-- <div class="pure-control-group">
                <label for="name">Foreign ID</label> -->
                <input type="hidden" name="foreign_id" id="foreign_id" value="<?php echo $foreign_id; ?>">
                <input type="hidden" name="doc_value" id="doc_value" value="<?php echo $value; ?>">
                <!-- <?php echo $foreign_id; ?>
            </div> -->
<br>
            <div class="pure-control-group">
                <label for="password">Statement record - choose one to settle:</label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                Show only: 
                        <?php echo '<a href="?action=add&foreign_id='.$foreign_id.'&foreign_table='.$foreign_table.'&value='.$value.'&date='.$date.'&desc='.$desc.'&acc_filter=359738937">359738937</a> | |
                                    <a href="?action=add&foreign_id='.$foreign_id.'&foreign_table='.$foreign_table.'&value='.$value.'&date='.$date.'&desc='.$desc.'&acc_filter=380251201">380251201</a> | |
                                    <a href="?action=add&foreign_id='.$foreign_id.'&foreign_table='.$foreign_table.'&value='.$value.'&date='.$date.'&desc='.$desc.'&acc_filter=380251201&bookers=1">380251201 with bookers</a> | |
                                    <a href="?action=add&foreign_id='.$foreign_id.'&foreign_table='.$foreign_table.'&value='.$value.'&date='.$date.'&desc='.$desc.'&acc_filter=PP: scubafroggy.com">PayPal</a><br>'; ?>
                <select name="bank_statement_id" id="bank_statement_id" autofocus size=25 style="width: 100em !important; min-width: 100em; max-width: 100em; font-size:0.9vw;" class='test-only'>
            
<?php

                foreach($bank_statement_array as $row)
                        {
                        if($value == $row['value'])
                            $style_color = '#ff0000';
                        else
                            $style_color = '#000000';


                        if($value == $row['value'] and $date == $row['value_date'])
                            $selected = 'selected';
                        else
                            $selected = '';

                        if($row['still_to_settle']<>0)
                            {
                            $max_sett_value = $row['still_to_settle'];
                            $stat_value = $formatter->formatCurrency($row['still_to_settle'], 'IDR').' (out of '.$formatter->formatCurrency($row['value'], 'IDR').')';
                            }
                        else
                            {
                            $max_sett_value = $row['value'];
                            $stat_value = $formatter->formatCurrency($row['value'], 'IDR');
                            }

                            //$formatter->formatCurrency($row['value'], 'IDR').' ('.$row['still_to_settle']

                        echo '<option value="'.$row['id_bank_statement'].'" maxsett="'.$max_sett_value.'" style="color: '.$style_color.'" '.$selected.'>'.$row['account_no'].' - '.$row['value_date'].': '.$stat_value.' '.$row['type'].' '.$row['description'].' </option>';
                        }
?>

                </select>
            </div>
<br>
<center>

            <div class="pure-control-group">
                <label for="name">Amount to be settled</label>
                <input type="text" name="value" id="value" value="<?php echo $value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-controls">
            <br>

                <?php
                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary" id="custom-submit-input" disabled="disabled">Submit</button>';

                   if($action<>'add')
                      echo '<a href="?action=del&id='.$id.'" class="pure-button button-error">Delete</a>';
                   }

                 ?>
</center>
            </div>
        </fieldset>
    </form>


</div>



<script type="text/javascript">
jQuery('.test-only').on('keyup change', function() {
 var flag = false;
  $('.test-only').each(function() {
       if($(this).val()=="") {
         flag = true;
         return false;
       }
  });

 jQuery('#custom-submit-input').prop('disabled', flag);

});

$(document).ready(function()
    {
    $('#bank_statement_id').change(function() 
        {
        var value = $(this).find("option:selected").attr("maxsett");
        var docvalue = $(this).getElementsByName('doc_value');
        //if(value<docvalue)
        //    $('#value').val(value);
        //if(value>=docvalue)
            $('#value').val(docvalue);
        
        });
    });

</script>