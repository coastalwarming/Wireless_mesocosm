
void Calculo_da_temp_a_enviar()
{
  byte temp_validas = 0;
  float soma = 0;


  for (int i = 1 ; i <= Viewers_size; i++) {
    if (Viewers[i].Received_state == 1 && Viewers[i].Temp_0 > -50 && Viewers[i].Temp_0 < 80) {
      //Serial.printf("Somei %f. \n", Viewers[i].Temp_0);
      soma += Viewers[i].Temp_0;
      temp_validas++;
    }
  }
  if (temp_validas > 0) {
    Master_struct.Target = (soma / temp_validas) + Temp_offset;
    Master_struct.Target_updated = true;
    Master_struct.No_Viewers = false;
  } else {
    Master_struct.Target = 0;
    Master_struct.Target_updated = false;
    Master_struct.No_Viewers = true;
    Serial.println("No_Viewers became true_2.");
  }

  //Serial.printf("Somei toal de %f. \n", soma);
  //Serial.printf("Iria enviar %f baseado em %d Viewers.\n", Master_struct.Target , temp_validas);
}
