/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern TIM_HandleTypeDef htim10;
#n/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
#t/* USER CODE BEGIN NonMaskableInt_IRQn 0 */
#n#t/* USER CODE END NonMaskableInt_IRQn 0 */
#t/* USER CODE BEGIN NonMaskableInt_IRQn 1 */
#n#t/* USER CODE END NonMaskableInt_IRQn 1 */
}#n
/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
#t/* USER CODE BEGIN HardFault_IRQn 0 */
#n#t/* USER CODE END HardFault_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_HardFault_IRQn 0 */
#t#t/* USER CODE END W1_HardFault_IRQn 0 */
#t}
#t/* USER CODE BEGIN HardFault_IRQn 1 */
#n#t/* USER CODE END HardFault_IRQn 1 */
}#n
/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
#t/* USER CODE BEGIN MemoryManagement_IRQn 0 */
#n#t/* USER CODE END MemoryManagement_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
#t#t/* USER CODE END W1_MemoryManagement_IRQn 0 */
#t}
#t/* USER CODE BEGIN MemoryManagement_IRQn 1 */
#n#t/* USER CODE END MemoryManagement_IRQn 1 */
}#n
/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
#t/* USER CODE BEGIN BusFault_IRQn 0 */
#n#t/* USER CODE END BusFault_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_BusFault_IRQn 0 */
#t#t/* USER CODE END W1_BusFault_IRQn 0 */
#t}
#t/* USER CODE BEGIN BusFault_IRQn 1 */
#n#t/* USER CODE END BusFault_IRQn 1 */
}#n
/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
#t/* USER CODE BEGIN UsageFault_IRQn 0 */
#n#t/* USER CODE END UsageFault_IRQn 0 */
#twhile (1)
#t{
#t#t/* USER CODE BEGIN W1_UsageFault_IRQn 0 */
#t#t/* USER CODE END W1_UsageFault_IRQn 0 */
#t}
#t/* USER CODE BEGIN UsageFault_IRQn 1 */
#n#t/* USER CODE END UsageFault_IRQn 1 */
}#n
/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
#t/* USER CODE BEGIN SVCall_IRQn 0 */
#n#t/* USER CODE END SVCall_IRQn 0 */
#t/* USER CODE BEGIN SVCall_IRQn 1 */
#n#t/* USER CODE END SVCall_IRQn 1 */
}#n
/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
#t/* USER CODE BEGIN DebugMonitor_IRQn 0 */
#n#t/* USER CODE END DebugMonitor_IRQn 0 */
#t/* USER CODE BEGIN DebugMonitor_IRQn 1 */
#n#t/* USER CODE END DebugMonitor_IRQn 1 */
}#n
/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
#t/* USER CODE BEGIN PendSV_IRQn 0 */
#n#t/* USER CODE END PendSV_IRQn 0 */
#t/* USER CODE BEGIN PendSV_IRQn 1 */
#n#t/* USER CODE END PendSV_IRQn 1 */
}#n
/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
#t/* USER CODE BEGIN SysTick_IRQn 0 */
#n#t/* USER CODE END SysTick_IRQn 0 */
#tHAL_IncTick();
#tHAL_SYSTICK_IRQHandler();
#t/* USER CODE BEGIN SysTick_IRQn 1 */
#n#t/* USER CODE END SysTick_IRQn 1 */
}#n

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
#t/* USER CODE BEGIN EXTI3_IRQn 0 */
#n#t/* USER CODE END EXTI3_IRQn 0 */
#tHAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
#t/* USER CODE BEGIN EXTI3_IRQn 1 */
#n#t/* USER CODE END EXTI3_IRQn 1 */
}#n
/**
* @brief This function handles EXTI line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
#t/* USER CODE BEGIN EXTI9_5_IRQn 0 */
#n#t/* USER CODE END EXTI9_5_IRQn 0 */
#tHAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
#t/* USER CODE BEGIN EXTI9_5_IRQn 1 */
#n#t/* USER CODE END EXTI9_5_IRQn 1 */
}#n
/**
* @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
*/
void TIM1_UP_TIM10_IRQHandler(void)
{
#t/* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */
#n#t/* USER CODE END TIM1_UP_TIM10_IRQn 0 */
#tHAL_TIM_IRQHandler(&htim10);
#t/* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */
#n#t/* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}#n
/**
* @brief This function handles DMA2 stream3 global interrupt.
*/
void DMA2_Stream3_IRQHandler(void)
{
#t/* USER CODE BEGIN DMA2_Stream3_IRQn 0 */
#n#t/* USER CODE END DMA2_Stream3_IRQn 0 */
get(halHandler) failed on instance of com.st.microxplorer.mcu.NvicVector
The problematic instruction:
----------
==> if-else [on line 202, column 1 in stm32xx_it_c.ftl]
----------
Java backtrace for programmers:
----------
freemarker.template.TemplateModelException: get(halHandler) failed on instance of com.st.microxplorer.mcu.NvicVector
at freemarker.ext.beans.BeanModel.get(BeanModel.java:223)
at freemarker.core.Dot._getAsTemplateModel(Dot.java:76)
at freemarker.core.Expression.getAsTemplateModel(Expression.java:89)
at freemarker.core.ComparisonExpression.isTrue(ComparisonExpression.java:111)
at freemarker.core.IfBlock.accept(IfBlock.java:80)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.MixedContent.accept(MixedContent.java:92)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.ConditionalBlock.accept(ConditionalBlock.java:79)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.MixedContent.accept(MixedContent.java:92)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.IteratorBlock$Context.runLoop(IteratorBlock.java:167)
at freemarker.core.Environment.visit(Environment.java:415)
at freemarker.core.IteratorBlock.accept(IteratorBlock.java:102)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.MixedContent.accept(MixedContent.java:92)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.Environment.visit(Environment.java:297)
at freemarker.core.CompressedBlock.accept(CompressedBlock.java:73)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.MixedContent.accept(MixedContent.java:92)
at freemarker.core.Environment.visit(Environment.java:208)
at freemarker.core.Environment.process(Environment.java:188)
at freemarker.template.Template.process(Template.java:237)
at com.st.microxplorer.codegenerator.CodeEngine.freemarkerDo(CodeEngine.java:266)
at com.st.microxplorer.codegenerator.CodeEngine.genCode(CodeEngine.java:187)
at com.st.microxplorer.codegenerator.CodeGenerator.generateOutputCode(CodeGenerator.java:2636)
at com.st.microxplorer.codegenerator.CodeGenerator.generatePeriphConfigCode(CodeGenerator.java:1528)
at com.st.microxplorer.codegenerator.CodeGenerator.generateCodeFiles(CodeGenerator.java:1188)
at com.st.microxplorer.codegenerator.CodeGenerator.generateCode(CodeGenerator.java:906)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.generateCode(ProjectBuilder.java:1056)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createCode(ProjectBuilder.java:940)
at com.st.microxplorer.plugins.projectmanager.engine.ProjectBuilder.createProject(ProjectBuilder.java:462)
at com.st.microxplorer.plugins.projectmanager.engine.GenerateProjectThread.run(GenerateProjectThread.java:41)
Caused by: java.lang.reflect.InvocationTargetException
at sun.reflect.GeneratedMethodAccessor108.invoke(Unknown Source)
at sun.reflect.DelegatingMethodAccessorImpl.invoke(Unknown Source)
at java.lang.reflect.Method.invoke(Unknown Source)
at freemarker.ext.beans.BeansWrapper.invokeMethod(BeansWrapper.java:835)
at freemarker.ext.beans.BeanModel.invokeThroughDescriptor(BeanModel.java:276)
at freemarker.ext.beans.BeanModel.get(BeanModel.java:183)
... 34 more
Caused by: java.lang.NullPointerException
at com.st.microxplorer.mcu.NvicVector.buildDmaIsr(NvicVector.java:1347)
at com.st.microxplorer.mcu.NvicVector.getHalHandler(NvicVector.java:288)
... 40 more