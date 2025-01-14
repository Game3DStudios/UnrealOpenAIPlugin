// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Provider/ResponseTypes.h"
#include "Provider/RequestTypes.h"
#include "AudioTranscriptionAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnAudioTranscription, const FAudioTranscriptionResponse&, Response, const FOpenAIError&, RawError);

class UOpenAIProvider;

UCLASS()
class UAudioTranscriptionAction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnAudioTranscription OnCompleted;

    virtual void Activate() override;

private:
    /**
     * @param URLOverride Allows for the specification of a custom endpoint. This is beneficial when using a proxy.
     * If this functionality is not required, this parameter can be left blank.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI | Audio")
    static UAudioTranscriptionAction* CreateAudioTranscription(
        const FAudioTranscription& AudioTranscription, const FOpenAIAuth& Auth, const FString& URLOverride);

    void TryToOverrideURL(UOpenAIProvider* Provider);

    void OnCreateAudioTranscriptionCompleted(const FAudioTranscriptionResponse& Response);
    void OnRequestError(const FString& URL, const FString& Content);

private:
    FAudioTranscription AudioTranscription;
    FOpenAIAuth Auth;
    FString URLOverride{};
};
